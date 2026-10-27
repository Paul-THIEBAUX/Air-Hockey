#pragma once

#include <string>
#include <DirectXMath.h>

class HUD
{
public:

	static void DrawHud();

	static void VictoryScreen();
	static void DrawScreen();
	static void LoseScreen();

	static void FromSentenceToHUD(int x, int y, const std::string& sentence, DirectX::XMFLOAT3 color);

	static void Alphabet(int x, int y, char letter, DirectX::XMFLOAT3 color);
	static void NumberTo5(int x, int y, int pt, DirectX::XMFLOAT3 color);
	static void Timer(int x, int y, float time, DirectX::XMFLOAT3 color);
	static void HUD_Number(bool up, bool Lhigh, bool Rhigh, bool mid, bool Llow, bool Rlow, bool down, int x, int y, DirectX::XMFLOAT3 color);

	static void HUD_Debug();

private:
};

