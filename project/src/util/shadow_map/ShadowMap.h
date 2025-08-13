#pragma once

namespace ShadowMap {

	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	void Init(int sizeX, int sizeY);

	/// <summary>
	/// ‰ğ•úˆ—
	/// </summary>
	void Release();

	/// <summary>
	/// ƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Å‰eŒvZ‚ğs‚¤
	/// </summary>
	void DrawBegin(int cameraIndex = 0);

	/// <summary>
	/// ‰e‚Ì•`‰æ‚ğI—¹‚·‚é
	/// </summary>
	void DrawEnd();

	/// <summary>
	/// •`‰æ‚Ég—p‚·‚é‰e‚Ìİ’è‚ğ‰ğœ‚·‚é
	/// </summary>
	void CleanUp();

	/// <summary>
	/// ‰e•`‰æ‚ğ“K‰‚·‚é
	/// </summary>
	/// <param name="">•`‰æŠÖ”</param>
	void Draw(void (*func)(), int cameraIndex = 0);

	/// <summary>
	/// ‰Ò“­‚·‚é‚©‚ğİ’è‚·‚é
	/// </summary>
	/// <param name="value">true‚È‚çUpdate‚âDraw‚Ìˆ—‚ğs‚¤‚±‚Æ‚ª‚Å‚«‚é</param>
	void SetIsActive(bool value);

	/// <summary>
	/// ‰Ò“­‚µ‚Ä‚¢‚é‚©‚ğæ“¾‚·‚é
	/// </summary>
	bool IsActive();
}