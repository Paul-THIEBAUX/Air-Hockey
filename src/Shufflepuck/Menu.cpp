#include "pch.h"
#include "Menu.h"
#include "HUD.h"

void Menu::Init() {

}

void Menu::Update() {
	if (GetisGameStarted() == true)
	{
		m_hideMenu -= 10000;
	}
	m_pHUD->FromSentenceToHUD(m_hideMenu + 200, m_hideMenu + 200, "Welcome to Shuffle Game!", CPU_WHITE);

	if (cpuInput.vi.IsKeyPressed(VK_DOWN)) {
		if (m_activeMenu == false)
			m_activeMenu = true;

	}
	if (cpuInput.vi.IsKeyPressed(VK_UP)) {
		if (m_activeMenu == true)
			m_activeMenu = false;

	}

	if (m_activeMenu == true) {
		m_pHUD->FromSentenceToHUD(m_hideMenu + 400, m_hideMenu + 300, " Start ", CPU_WHITE);
		m_pHUD->FromSentenceToHUD(m_hideMenu + 400, m_hideMenu + 450, ">Leave<", CPU_WHITE);
	}
	else {
		m_pHUD->FromSentenceToHUD(m_hideMenu + 400, m_hideMenu + 300, ">Start<", CPU_WHITE);
		m_pHUD->FromSentenceToHUD(m_hideMenu + 400, m_hideMenu + 450, " Leave ", CPU_WHITE);
	}

	if (cpuInput.vi.IsKeyPressed(VK_RETURN)) {
		if (m_activeMenu == false) {
			ButtonStart();
		}
		else if (m_activeMenu == true) {
			ButtonQuit();
		}
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
