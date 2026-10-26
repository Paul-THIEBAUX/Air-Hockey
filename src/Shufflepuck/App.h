#pragma once

#include "Camera.h"
#include "SceneArt.h"
#include "ShuffleGame.h"
#include "HUD.h"


class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;
	SceneArt* m_pSceneArt = nullptr;
	Camera* m_pCamera = nullptr;

	shuffle::Game m_game;
	float m_pMax = 50.0f;

};
