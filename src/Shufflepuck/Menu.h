#pragma once
class Menu
{
public:
	void Init();
	void Update();

	void ButtonQuit();
	void ButtonStart();

	const bool& GetisGameStarted()   const { return m_isGameStarted; } // True = Game Started, False = Menu Active

private:
	HUD* m_pHUD = nullptr;
	bool m_activeMenu = true;
	bool m_isGameStarted = false;
	int m_hideMenu = 0;
};

