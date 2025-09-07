#pragma once

#include <string> 
#include <sstream>
#include <windows.h>
#include <commctrl.h> 
#include <tchar.h>
#include "resource.h"


static std::string* nameText = nullptr;
static bool isNameEntry = false;
static LRESULT CALLBACK DlgProc1(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

    TCHAR buf[32];
    switch (msg) {
    case WM_INITDIALOG:
        isNameEntry = false;
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), _TEXT("NoName"));
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_EDIT1:
            if (HIWORD(wParam) == EN_UPDATE) {	//	エディットボックスが変更された場合
                SetWindowText(GetDlgItem(hDlg, IDC_LABEL1), _TEXT("名前を入れてください"));
            }
            break;
        case IDOK:
            GetDlgItemText(hDlg, IDC_EDIT1, (TCHAR*)buf, sizeof(buf) / sizeof(TCHAR));
            MessageBox(hDlg, buf, _TEXT("登録完了。 請求額: ￥50,000"), MB_OK);
            EndDialog(hDlg, TRUE);
            if (not nameText)
                nameText = new std::string(buf);
            isNameEntry = true;
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return FALSE;
        default:
            return FALSE;
        }
    default:
        return FALSE;
    }
    return TRUE;
}