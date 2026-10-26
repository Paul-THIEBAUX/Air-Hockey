#pragma once

class HUD
{
public:

	void DrawHud();

	void VictoryScreen();
	void DrawScreen();
	void LoseScreen();

	void FromSentenceToHUD(int x, int y, const std::string& sentence, DirectX::XMFLOAT3 color);

	void Alphabet(int x, int y, char letter, DirectX::XMFLOAT3 color);
	void NumberTo5(int x, int y, int pt, DirectX::XMFLOAT3 color);
	void Timer(int x, int y, float time, DirectX::XMFLOAT3 color);
	void HUD_Number(bool up, bool Lhigh, bool Rhigh, bool mid, bool Llow, bool Rlow, bool down, int x, int y, DirectX::XMFLOAT3 color);

	void HUD_Debug();

private:
};

