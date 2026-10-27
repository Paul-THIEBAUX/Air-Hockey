#pragma once
class Menu
{
public:
	void Init();
	void Update();
	void Draw();

	void ButtonQuit();
	void ButtonStart();

	const bool& GetisGameStarted()   const { return m_isGameStarted; } // True = Game Started, False = Menu Active

private:
	bool m_activeMenu = true;
	bool m_isGameStarted = false;
};

