#include "pch.h"
#include "HUD.h"


void HUD::DrawHud()
{
	const float h = 0.0f;
	//cadre
	cpuDevice.DrawLine(0, 0, h, 450, 0, h, CPU_WHITE);

	cpuDevice.DrawLine(0, 50, h, 450, 50, h, CPU_WHITE);
	cpuDevice.DrawLine(0, 100, h, 400, 100, h, CPU_WHITE);
	cpuDevice.DrawLine(0, 150, h, 200, 150, h, CPU_WHITE);

	cpuDevice.DrawLine(450, 50, h, 450, 0, h, CPU_WHITE);
	cpuDevice.DrawLine(400, 100, h, 450, 50, h, CPU_WHITE);
	cpuDevice.DrawLine(200, 150, h, 250, 100, h, CPU_WHITE);

	cpuDevice.DrawLine(0, 0, h, 0, 150, h, CPU_WHITE);

	//nom
	const float m = 10;
	const float e = 25;
	const float l = 50;

	const std::string& joueur = "player";
	const std::string& ia = "bot";
	const std::string& time = "Time";

	FromSentenceToHUD(m, m + l * 0, joueur, CPU_WHITE); //JOUEUR
	FromSentenceToHUD(m, m + l * 1, ia, CPU_WHITE); //IA
	FromSentenceToHUD(m, m + l * 2, time, CPU_WHITE); //TIME

}

void HUD::VictoryScreen() {
	// message : You win \n well done

	const std::string& Main = "You win.";
	const std::string& sub = "Well done.";
	FromSentenceToHUD(400, 250, Main, CPU_BLUE);
	FromSentenceToHUD(400, 300, sub, CPU_BLUE);
}
void HUD::DrawScreen() {
	// message : Draw

	const std::string& Main = "Draw.";
	const std::string& sub = "So close.";
	FromSentenceToHUD(400, 250, Main, CPU_ORANGE);
	FromSentenceToHUD(400, 300, sub, CPU_ORANGE);
}
void HUD::LoseScreen() {
	// message : You Lose \n Better luck next time

	const std::string& Main = "You lose.";
	const std::string& sub = "Better luck next time.";
	FromSentenceToHUD(400, 250, Main, CPU_RED);
	FromSentenceToHUD(200, 300, sub, CPU_RED);
}

void HUD::FromSentenceToHUD(int x, int y, const std::string& sentence, DirectX::XMFLOAT3 color) {
	for (char c : sentence) { //stolen from internet
		Alphabet(x, y, c, color);
		x += 25;
	}
}

void HUD::Alphabet(int x, int y, char letter, DirectX::XMFLOAT3 color) {
	if (letter == ' ') {
		HUD_Number(0, 0, 0, 0, 0, 0, 0, x, y, color);
	}
	if (letter == '.') {
		cpuDevice.DrawLine(x + 6, y + 30, 0.0f, x + 7, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 30, 0.0f, x + 7, y + 29, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 29, 0.0f, x + 6, y + 29, 0.0f, color);
		cpuDevice.DrawLine(x + 6, y + 29, 0.0f, x + 6, y + 30, 0.0f, color);
	}
	if (letter == ':') {
		cpuDevice.DrawLine(x + 6, y + 10, 0.0f, x + 7, y + 10, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 10, 0.0f, x + 7, y + 9, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 9, 0.0f, x + 6, y + 9, 0.0f, color);
		cpuDevice.DrawLine(x + 6, y + 9, 0.0f, x + 6, y + 10, 0.0f, color);
		cpuDevice.DrawLine(x + 6, y + 20, 0.0f, x + 7, y + 20, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 20, 0.0f, x + 7, y + 19, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 19, 0.0f, x + 6, y + 19, 0.0f, color);
		cpuDevice.DrawLine(x + 6, y + 19, 0.0f, x + 6, y + 20, 0.0f, color);
	}
	if (letter == '>') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 15, 0.0f, x + 0, y + 30, 0.0f, color);
	}
	if (letter == '<') {
		cpuDevice.DrawLine(x + 15, y + 0, 0.0f, x + 0, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 15, 0.0f, x + 15, y + 30, 0.0f, color);
	}

	if (letter == '0') {
		HUD_Number(1, 1, 1, 0, 1, 1, 1, x, y, color);
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	if (letter == '1') {
		HUD_Number(0, 0, 1, 0, 0, 1, 0, x, y, color);
	}
	if (letter == '2') {
		HUD_Number(1, 0, 1, 1, 1, 0, 1, x, y, color);
	}
	if (letter == '3') {
		HUD_Number(1, 0, 1, 1, 0, 1, 1, x, y, color);
	}
	if (letter == '4') {
		HUD_Number(0, 1, 1, 1, 0, 1, 0, x, y, color);
	}
	if (letter == '5') {
		HUD_Number(1, 1, 0, 1, 0, 1, 1, x, y, color);
	}
	if (letter == '6') {
		HUD_Number(1, 1, 0, 1, 1, 1, 1, x, y, color);
	}
	if (letter == '7') {
		HUD_Number(1, 0, 1, 0, 0, 1, 0, x, y, color);
	}
	if (letter == '8') {
		HUD_Number(1, 1, 1, 1, 1, 1, 1, x, y, color);
	}
	if (letter == '9') {
		HUD_Number(1, 1, 1, 1, 0, 1, 1, x, y, color);
	}


	if (letter == 'A' || letter == 'a') {
		HUD_Number(1, 1, 1, 1, 1, 1, 0, x, y, color);
	}
	else if (letter == 'B' || letter == 'b') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 5, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 10, y + 2, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 2, 0.0f, x + 10, y + 8, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 8, 0.0f, x + 7, y + 10, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 10, 0.0f, x + 15, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 15, 0.0f, x + 15, y + 25, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 25, 0.0f, x + 10, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 30, 0.0f, x + 0, y + 30, 0.0f, color);
	}
	else if (letter == 'C' || letter == 'c') {
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 0, y + 5, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 5, 0.0f, x + 0, y + 25, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 25, 0.0f, x + 5, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 30, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	else if (letter == 'D' || letter == 'd') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 10, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 0, 0.0f, x + 15, y + 5, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 5, 0.0f, x + 15, y + 25, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 25, 0.0f, x + 10, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 30, 0.0f, x + 0, y + 30, 0.0f, color);
	}
	else if (letter == 'E' || letter == 'e') {
		HUD_Number(1, 1, 0, 1, 1, 0, 1, x, y, color);
	}
	else if (letter == 'F' || letter == 'f') {
		HUD_Number(1, 1, 0, 1, 1, 0, 0, x, y, color);
	}
	else if (letter == 'G' || letter == 'g') {
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 0, y + 5, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 5, 0.0f, x + 0, y + 25, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 25, 0.0f, x + 5, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 30, 0.0f, x + 10, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 10, y + 30, 0.0f, x + 15, y + 25, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 25, 0.0f, x + 15, y + 20, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 20, 0.0f, x + 10, y + 20, 0.0f, color);
	}
	else if (letter == 'H' || letter == 'h') {
		HUD_Number(0, 1, 1, 1, 1, 1, 0, x, y, color);
	}
	else if (letter == 'I' || letter == 'i') {
		cpuDevice.DrawLine(x + 5, y + 0, 0.0f, x + 10, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 0, 0.0f, x + 7, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 5, y + 30, 0.0f, x + 10, y + 30, 0.0f, color);
	}
	else if (letter == 'J' || letter == 'j') {
		HUD_Number(0, 0, 1, 0, 1, 1, 1, x, y, color);
	}
	else if (letter == 'K' || letter == 'k') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 15, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 15, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	else if (letter == 'L' || letter == 'l') {
		HUD_Number(0, 1, 0, 0, 1, 0, 1, x, y, color);
	}
	else if (letter == 'M' || letter == 'm') {
		cpuDevice.DrawLine(x + 0, y + 30, 0.0f, x + 0, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 7, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 15, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 0, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	else if (letter == 'N' || letter == 'n') {
		cpuDevice.DrawLine(x + 0, y + 30, 0.0f, x + 0, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 30, 0.0f, x + 15, y + 0, 0.0f, color);
	}
	else if (letter == 'O' || letter == 'o') {
		HUD_Number(1, 1, 1, 0, 1, 1, 1, x, y, color);
	}
	else if (letter == 'P' || letter == 'p') {
		HUD_Number(1, 1, 1, 1, 1, 0, 0, x, y, color);
	}
	else if (letter == 'Q' || letter == 'q') {
		HUD_Number(1, 1, 1, 0, 1, 1, 1, x, y, color);
		cpuDevice.DrawLine(x + 10, y + 20, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	else if (letter == 'R' || letter == 'r') {
		HUD_Number(1, 1, 1, 1, 1, 0, 0, x, y, color);
		cpuDevice.DrawLine(x + 0, y + 15, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	else if (letter == 'S' || letter == 's') {
		HUD_Number(1, 1, 0, 1, 0, 1, 1, x, y, color);
	}
	else if (letter == 'T' || letter == 't') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 0, 0.0f, x + 7, y + 30, 0.0f, color);
	}
	else if (letter == 'U' || letter == 'u') {
		HUD_Number(0, 1, 1, 0, 1, 1, 1, x, y, color);
	}
	else if (letter == 'V' || letter == 'v') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 7, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 30, 0.0f, x + 15, y + 0, 0.0f, color);
	}
	else if (letter == 'W' || letter == 'w') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 30, 0.0f, x + 7, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 7, y + 15, 0.0f, x + 15, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 30, 0.0f, x + 15, y + 0, 0.0f, color);
	}
	else if (letter == 'X' || letter == 'x') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
	}
	else if (letter == 'Y' || letter == 'y') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, (int)x + 15 * 0.5f, y + 15, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
	}
	else if (letter == 'Z' || letter == 'z') {
		cpuDevice.DrawLine(x + 0, y + 0, 0.0f, x + 15, y + 0, 0.0f, color);
		cpuDevice.DrawLine(x + 15, y + 0, 0.0f, x + 0, y + 30, 0.0f, color);
		cpuDevice.DrawLine(x + 0, y + 30, 0.0f, x + 15, y + 30, 0.0f, color);
	}
}

void HUD::NumberTo5(int x, int y, int pt, DirectX::XMFLOAT3 color) {
	if (pt == 0) {
		Alphabet(x, y, '0', color);
	}
	else if (pt == 1) {
		Alphabet(x, y, '1', color);
	}
	else if (pt == 2) {
		Alphabet(x, y, '2', color);
	}
	else if (pt == 3) {
		Alphabet(x, y, '3', color);
	}
	else if (pt == 4) {
		Alphabet(x, y, '4', color);
	}
	else if (pt == 5) {
		Alphabet(x, y, '5', color);
	}
}

void HUD::Timer(int x, int y, float time, DirectX::XMFLOAT3 color) {
	int pt = (int)time;
	if (pt == 0) {
		Alphabet(x, y, '0', color);
	}
	else if (pt == 1) {
		Alphabet(x, y, '1', color);
	}
	else if (pt == 2) {
		Alphabet(x, y, '2', color);
	}
	else if (pt == 3) {
		Alphabet(x, y, '3', color);
	}
	else if (pt == 4) {
		Alphabet(x, y, '4', color);
	}
	else if (pt == 5) {
		Alphabet(x, y, '5', color);
	}
	else if (pt == 6) {
		Alphabet(x, y, '6', color);
	}
	else if (pt == 7) {
		Alphabet(x, y, '7', color);
	}
	else if (pt == 8) {
		Alphabet(x, y, '8', color);
	}
	else if (pt == 9) {
		Alphabet(x, y, '9', color);
	}
	else if (pt == 0) {
		Alphabet(x, y, '0', color);
	}
}

/*
1 = 0,0,1,0,0,1,0
2 = 1,0,1,1,1,0,1
3 = 1,0,1,1,0,1,1
4 = 0,1,1,1,0,1,0
5 = 1,1,0,1,0,1,1
6 = 1,1,0,1,1,1,1
7 = 1,0,1,0,0,1,0
8 = 1,1,1,1,1,1,1
9 = 1,1,1,1,0,1,1
0 = 1,1,1,0,1,1,1
*/
void HUD::HUD_Number(bool up, bool Lhigh, bool Rhigh, bool mid, bool Llow, bool Rlow, bool down, int x, int y, DirectX::XMFLOAT3 color) {
	//2;3;5;6;7;8;9
	if (up == true) {
		cpuDevice.DrawLine(x, y, 0.0f, x + 15, y, 0.0f, color);
	}
	//4;5;6;8;9
	if (Lhigh == true) {
		cpuDevice.DrawLine(x, y, 0.0f, x, y + 15, 0.0f, color);
	}
	//1;2;3;4;7;8;9
	if (Rhigh == true) {
		cpuDevice.DrawLine(x + 15, y, 0.0f, x + 15, y + 15, 0.0f, color);
	}
	//2;3;4;5;6;8;9
	if (mid == true) {
		cpuDevice.DrawLine(x, y + 15, 0.0f, x + 15, y + 15, 0.0f, color);
	}
	//2;6;8
	if (Llow == true) {
		cpuDevice.DrawLine(x, y + 15, 0.0f, x, y + 30, 0.0f, color);
	}
	//1;3;4;5;6;7;8;9
	if (Rlow == true) {
		cpuDevice.DrawLine(x + 15, y + 15, 0.0f, x + 15, y + 30, 0.0f, color);
	}
	//2;3;5;6;8;9
	if (down == true) {
		cpuDevice.DrawLine(x, y + 30, 0.0f, x + 15, y + 30, 0.0f, color);
	}
}

void HUD::HUD_Debug()
{
	for (int i = 0; i < 1200; i += 100)
	{
		int x = i + 50;
		int len50 = 5;
		int len100 = 10;

		cpuDevice.DrawLine(x, 0, 0.0f, x, len50, 0.0f, CPU_GREEN);
		cpuDevice.DrawLine(0, x, 0.0f, len50, x, 0.0f, CPU_RED);

		cpuDevice.DrawLine(i, 0, 0.0f, i, len100, 0.0f, CPU_GREEN);
		cpuDevice.DrawLine(0, i, 0.0f, len100, i, 0.0f, CPU_RED);
	}//1024, 576
	cpuDevice.DrawLine(1024 * 0.5, 576 * 0.5 - 25, 0.0f, 1024 * 0.5, 576 * 0.5 + 25, 0.0f, CPU_WHITE);
	cpuDevice.DrawLine(1024 * 0.5 - 25, 576 * 0.5, 0.0f, 1024 * 0.5 + 25, 576 * 0.5, 0.0f, CPU_WHITE);
}