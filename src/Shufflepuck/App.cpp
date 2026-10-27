#include "pch.h"

App::App()
{


	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	cpuInput.MapActionForVI(0, VK_RETURN, VK_SPACE);
	cpuInput.MapActionForXI(0, CPU_XINPUT_A);

	m_pCamera = new Camera();
	m_pCamera->Init();

	m_pSceneArt = new SceneArt();
	m_pSceneArt->createPlateau();

	m_game.Start();

#ifdef _DEBUG 
	m_pSceneArt->createDebugOrthonorme();

#endif

}

void App::OnUpdate()
{
	m_game.UpdateMenu();
	m_pCamera->Update();
	const XMFLOAT3& puck = m_game.GetPuck().pos;
	const XMFLOAT3& player = m_game.GetPlayer().pos;
	const XMFLOAT3& ai = m_game.GetAI().pos;
	if (m_game.PuckHidden()) m_pSceneArt->GetBall()->transform.SetPosition(0.0f, -1000.0f, 0.0f);   //an apple is drawn instead
	else m_pSceneArt->GetBall()->transform.SetPosition(puck.x, puck.y, puck.z);
	m_pSceneArt->GetPallet()->transform.SetPosition(player.x, player.y, player.z);
	m_pSceneArt->GetEnnemi()->transform.SetPosition(ai.x, ai.y, ai.z);
	

	// Quit
	if (cpuInput.vi.IsKeyPressed(VK_F10))
		cpuEngine.Quit();

	//debug
#ifdef _DEBUG 
	if (cpuInput.vi.IsKeyPressed(VK_F5))
		m_game.SwitchStopStart();
#endif

}

void App::OnExit()
{
	CPU_DELPTR(m_pSceneArt);
	CPU_DELPTR(m_pCamera);
}

void App::OnRender(int pass)
{
	if (pass != CPU_PASS_UI_BEGIN)
	{
		return;
	}

	HUD::DrawHud();
	m_game.DrawVictoryMessage();
	m_game.PointDisplay();
	m_game.TimerDisplay();
	m_game.DrawInputMode();
	m_game.DrawMenu();

#ifdef _DEBUG 
	HUD::HUD_Debug();
#endif
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
