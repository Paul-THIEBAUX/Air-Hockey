#include "pch.h"
#include "Menu.h"
#include "HUD.h"

void Menu::Init() {

}

void Menu::Update() {
	if (cpuInput.vi.IsKeyPressed(VK_DOWN) || cpuInput.xi.IsDownPressed()) {
		m_activeMenu = true;
	}
	if (cpuInput.vi.IsKeyPressed(VK_UP) || cpuInput.xi.IsUpPressed()) {
		m_activeMenu = false;
	}

	bool validate = cpuInput.vi.IsKeyPressed(VK_RETURN)
		|| cpuInput.vi.IsKeyPressed(VK_SPACE)
		|| cpuInput.xi.IsButtonPressed(CPU_XINPUT_A);
	if (validate) {
		if (m_activeMenu == false) {
			ButtonStart();
		}
		else {
			ButtonQuit();
		}
	}
}

void Menu::Draw() {
	HUD::FromSentenceToHUD(200, 200, "Welcome to Shuffle Game!", CPU_WHITE);

	if (m_activeMenu == true) {
		HUD::FromSentenceToHUD(400, 300, " Start ", CPU_WHITE);
		HUD::FromSentenceToHUD(400, 450, ">Leave<", CPU_WHITE);
	}
	else {
		HUD::FromSentenceToHUD(400, 300, ">Start<", CPU_WHITE);
		HUD::FromSentenceToHUD(400, 450, " Leave ", CPU_WHITE);
	}
}

void Menu::ButtonQuit() {
	cpuEngine.Quit();
}

void Menu::ButtonStart() {
	cpuEngine.GetCamera()->transform.SetPosition(20.0f, 10.0f, 0.0f);
	cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	m_isGameStarted = true;
}
