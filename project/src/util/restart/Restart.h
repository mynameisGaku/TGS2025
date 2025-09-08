#pragma once

#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <processthreadsapi.h>

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

///<summary>
/// 再起動するマン
///</summary>
///<author>藤本樂</author>
class Restart
{
public:
	///<summary>
	/// 起動直後に呼び出し。コマンドラインに "--wait-parent <pid>" があれば、その PID が終了するまで待機する。
	/// @param argc main/WinMain から受けた argc
	/// @param argv main/WinMain から受けた argv
	///</summary>
	static void HandleWaitParentIfNeeded(int argc, char* argv[])
	{
		int idx = FindWaitParentIndex(argc, argv);
		if (idx < 0 || idx + 1 >= argc)
		{
			return;
		}
		const DWORD parentPid = static_cast<DWORD>(std::strtoll(argv[idx + 1], nullptr, 10));
		if (parentPid == 0)
		{
			return;
		}

		HANDLE hProc = OpenProcess(SYNCHRONIZE, FALSE, parentPid);
		if (hProc != nullptr)
		{
			// 親が終了するまで待機
			WaitForSingleObject(hProc, INFINITE);
			CloseHandle(hProc);
		}
		// プロセスが既に居ない等は無視して続行
	}

	///<summary>
	/// 再起動要求。現在の exe と引数を引き継ぎつつ、新プロセスを起動する(--wait-parent <自分PID> を付与)。
	/// 呼び出し元では true が返ったら「設定保存→DxLib_End()→return」で終了すること。
	/// @param argc main/WinMain から受けた argc
	/// @param argv main/WinMain から受けた argv
	/// @return 起動成功で true。失敗時は false(この場合は続行するか、エラー表示して中止)。
	///</summary>
	static bool RequestRestart(int argc, char* argv[])
	{
		std::wstring exePath = GetExecutablePathW();
		if (exePath.empty())
		{
			std::fputs("GetModuleFileNameW failed.\n", stderr);
			return false;
		}

		// 元の引数に "--wait-parent <pid>" を追加
		std::vector<std::wstring> argsW;
		argsW.reserve(static_cast<size_t>(argc) + 2);
		for (int i = 1; i < argc; ++i)
		{
			argsW.emplace_back(Utf8ToWide(argv[i]));
		}
		argsW.emplace_back(L"--wait-parent");
		argsW.emplace_back(std::to_wstring(GetCurrentProcessId()));

		// CreateProcessW 用のコマンドライン文字列を構築(正しいクォートを行う)
		std::wstring cmd = QuoteArgW(exePath);
		for (const auto& a : argsW)
		{
			cmd.push_back(L' ');
			cmd += QuoteArgW(a);
		}

		STARTUPINFOW si{};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi{};
		// 注意：CreateProcessW は第2引数を書き換える可能性があるので、可変バッファを渡す
		std::wstring cmdBuf = cmd;

		BOOL ok = CreateProcessW(
			exePath.c_str(),         // lpApplicationName
			cmdBuf.data(),           // lpCommandLine (可変バッファ)
			nullptr,                 // lpProcessAttributes
			nullptr,                 // lpThreadAttributes
			FALSE,                   // bInheritHandles
			CREATE_NEW_PROCESS_GROUP,// dwCreationFlags(任意)
			nullptr,                 // lpEnvironment
			nullptr,                 // lpCurrentDirectory
			&si, &pi
		);

		if (!ok)
		{
			const DWORD err = GetLastError();
			char buf[256];
			std::snprintf(buf, sizeof(buf), "CreateProcessW failed. GetLastError()=%lu\n", static_cast<unsigned long>(err));
			std::fputs(buf, stderr);
			return false;
		}

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}

	///<summary>
	/// 再起動を要求し、成功したらクリーンアップ関数を呼んで即終了する。
	/// @param argc argc
	/// @param argv argv
	/// @param cleanup 後始末用コールバック(設定保存、スレッド停止、DxLib_End など)。nullptr 可。
	/// @return 再起動を発行して終了した場合は 0 を返す(そのまま return で使う)。失敗時は -1。
	///</summary>
	static int RestartAndExit(int argc, char* argv[], void(*cleanup)())
	{
		if (RequestRestart(argc, argv))
		{
			if (cleanup)
			{
				cleanup();
			}
			// ここで確実に終了(呼び出し側で return 0 してもOK)
			ExitProcess(0);
			return 0; // 到達しないが、静的解析対策
		}
		return -1;
	}

private:
	///<summary>
	/// "--wait-parent" のインデックスを探す(見つからなければ -1)
	///</summary>
	static int FindWaitParentIndex(int argc, char* argv[])
	{
		for (int i = 1; i < argc; ++i)
		{
			if (std::strcmp(argv[i], "--wait-parent") == 0)
			{
				return i;
			}
		}
		return -1;
	}

	///<summary>
	/// EXE のフルパス(Unicode)を取得
	///</summary>
	static std::wstring GetExecutablePathW()
	{
		std::wstring path;
		path.resize(32768);
		DWORD len = GetModuleFileNameW(nullptr, &path[0], static_cast<DWORD>(path.size()));
		if (len == 0)
		{
			return L"";
		}
		path.resize(len);
		return path;
	}

	///<summary>
	/// UTF-8 → UTF-16 の簡易変換(引数継承用)
	///</summary>
	static std::wstring Utf8ToWide(const char* s)
	{
		if (!s)
		{
			return L"";
		}
		int wlen = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
		if (wlen <= 0)
		{
			return L"";
		}
		std::wstring w;
		w.resize(static_cast<size_t>(wlen - 1));
		if (wlen > 1)
		{
			MultiByteToWideChar(CP_UTF8, 0, s, -1, w.data(), wlen);
		}
		return w;
	}

	///<summary>
	/// Windows のコマンドライン引数クォート規則に従って安全にクォート
	///</summary>
	static std::wstring QuoteArgW(const std::wstring& arg)
	{
		// 空白/タブ/ダブルクォートが無ければそのまま
		if (arg.find_first_of(L" \t\"") == std::wstring::npos)
		{
			return arg;
		}

		std::wstring result = L"\"";
		size_t backslashes = 0;
		for (wchar_t ch : arg)
		{
			if (ch == L'\\')
			{
				++backslashes;
			}
			else if (ch == L'"')
			{
				// 直前のバックスラッシュは 2 倍し、クォート自体をエスケープ
				result.append(backslashes * 2 + 1, L'\\');
				result.push_back(L'"');
				backslashes = 0;
			}
			else
			{
				if (backslashes > 0)
				{
					result.append(backslashes, L'\\');
					backslashes = 0;
				}
				result.push_back(ch);
			}
		}
		if (backslashes > 0)
		{
			result.append(backslashes * 2, L'\\');
		}
		result.push_back(L'"');
		return result;
	}
};