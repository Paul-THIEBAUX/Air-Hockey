#include "pch.h"
#include "ShuffleGame.h"

namespace shuffle
{
	struct Game::StateGlobal
	{
		void OnEnter(Game&, int, void*) {}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{
			g.m_input.UpdateToggle();
			g.UpdateAvatar();

			//F8 switch
			if (BADAPPLE_DEBUG && cpuInput.vi.IsKeyPressed(VK_F8))
			{
				if (g.m_opponent == OpponentId::Normal)
				{
					g.SetOpponent(OpponentId::BadApple);
				}
				else
				{
					g.SetOpponent(OpponentId::Normal);
				}
				return;
			}

			//Pause
			if (!cpuInput.vi.IsKeyPressed(VK_ESCAPE))
			{
				return;
			}
			int state = g.m_pFSM->state;
			if (state == CPU_ID(StatePaused))
			{
				g.m_pFSM->ToState(g.m_resumeState);
			}
			else if (state == CPU_ID(StateServe) || state == CPU_ID(StatePlaying))
			{
				g.m_resumeState = state;
				g.m_pFSM->ToState(CPU_ID(StatePaused));
			}
		}
	};

	struct Game::StateSetup
	{
		void OnEnter(Game&, int, void*) {}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{
			if (g.m_pFSM->totalTime >= SETUP_TIME)
			{
				g.m_pFSM->ToState(CPU_ID(StateServe));
			}
		}
	};

	struct Game::StateServe
	{
		void OnEnter(Game& g, int from, void*)
		{
			//resuming from pause must not reset the puck
			//flashback of the AI project
			if (from != CPU_ID(StatePaused))
			{
				g.NewRound(g.m_serveToPlayer);
			}
		}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{
			g.MovePlayer(cpuTime.delta);
			if (g.m_pFSM->totalTime >= SERVE_DELAY)
			{
				g.m_pFSM->ToState(CPU_ID(StatePlaying));
			}
		}
	};

	struct Game::StatePlaying
	{
		void OnEnter(Game& g, int from, void*)
		{
			if (from == CPU_ID(StateServe))
			{
				float direction = -1.0f;
				if (g.m_serveToPlayer)
				{
					direction = 1.0f;
				}
				g.m_puck.vel = { direction * 5.0f, 0.0f, 0.0f };
			}
		}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{

			float dt = fminf(cpuTime.delta, 1.0f / 30.0f);

			//the paddles
			g.MovePlayer(dt);
			XMFLOAT3 aiTarget = g.m_brain->Think(g.m_puck, g.m_ai, dt);
			float aiMaxSpeed = PADDLE_MAX_SPEED * g.m_brain->SpeedFactor();
			Physics::MovePaddle(g.m_ai, aiTarget, dt, aiMaxSpeed);

			//puck
			StepEvents ev = g.m_physics.Step(g.m_puck, g.m_player, g.m_ai, dt);
			g.m_match.Tick(dt);

			if (ev.goal != Goal::None)
			{
				g.m_match.OnGoal(ev.goal);
				g.m_lastGoal = ev.goal;
				g.m_serveToPlayer = (ev.goal == Goal::AI);   //the conceder receives the puck
				g.m_pFSM->ToState(CPU_ID(StateGoal));
			}
			else if (g.m_match.IsOver())
			{
				g.m_pFSM->ToState(CPU_ID(StateGameOver));   //time is up
			}
		}
	};

	struct Game::StateGoal
	{
		void OnEnter(Game&, int, void*) {}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{
			if (g.m_match.IsOver())
			{
				g.m_pFSM->ToState(CPU_ID(StateGameOver));
			}
			else if (g.m_pFSM->totalTime >= GOAL_PAUSE)
			{
				g.m_pFSM->ToState(CPU_ID(StateServe));
			}
		}
	};

	struct Game::StateGameOver
	{
		void OnEnter(Game&, int, void*) {}
		void OnExit(Game&, int) {}
		void OnExecute(Game& g)
		{
			if (cpuInput.IsActionPressed())
			{
				g.Start();   //restart
			}
		}
	};

	struct Game::StatePaused
	{
		void OnEnter(Game&, int, void*) {}
		void OnExit(Game&, int) {}
		void OnExecute(Game&) {}
	};

	void Game::Start()
	{
		if (!m_pFSM)
		{
			m_pFSM = cpuEngine.CreateFSM(this);
			m_pFSM->SetPreGlobal<StateGlobal>();
			m_pFSM->Add<StateSetup>("Setup");
			m_pFSM->Add<StateServe>("Serve");
			m_pFSM->Add<StatePlaying>("Playing");
			m_pFSM->Add<StateGoal>("Goal");
			m_pFSM->Add<StateGameOver>("GameOver");
			m_pFSM->Add<StatePaused>("Paused");
		}
		SelectAvatar();
		m_match.Reset();
		m_serveToPlayer = true;
		m_lastGoal = Goal::None;

		//Player
		m_player.pos = { TABLE_L * 0.25f, TABLE_Y, 0.0f };
		m_player.vel = { 0.0f, 0.0f, 0.0f };
		m_player.minX = PADDLE_R;
		m_player.maxX = TABLE_L * 0.5f - BORDER_R - PADDLE_R;

		//AI
		m_ai.pos = { -TABLE_L * 0.25f, TABLE_Y, 0.0f };
		m_ai.vel = { 0.0f, 0.0f, 0.0f };
		m_ai.minX = -TABLE_L * 0.5f + BORDER_R + PADDLE_R;
		m_ai.maxX = -PADDLE_R;

		m_physics.Reset(m_puck, 0.0f, { 0.0f, 0.0f, 0.0f });
		m_pFSM->ToState(CPU_ID(StateSetup));
	}

	void Game::NewRound(bool) { m_physics.Reset(m_puck, 0.0f, { 0,0,0 }); }

	void Game::MovePlayer(float dt)
	{

		XMFLOAT3 target;
		if (m_input.GetTarget(m_player, dt, target))
		{
			Physics::MovePaddle(m_player, target, dt);
		}
	}

	void Game::UpdateAvatar()
	{
		if (!m_avatar) return;
		AvatarInfo info;
		info.puck = m_puck.pos;
		info.mood = GetAIMood();
		m_avatar->Update(cpuTime.delta, info);
	}

	AIAvatar* Game::GetOrBuildAvatar(OpponentId id)
	{
		int index = (int)id;
		if (!m_avatars[index])
		{
			m_avatars[index].reset(m_brain->CreateAvatar());
			m_avatars[index]->Build();
		}
		return m_avatars[index].get();
	}

	void Game::SelectAvatar()
	{
		AIAvatar* pAvatar = GetOrBuildAvatar(m_opponent);

		if (!pAvatar->IsReady())
		{
			m_opponent = OpponentId::Normal;
			m_brain = AI::Create(m_opponent);
			pAvatar = GetOrBuildAvatar(m_opponent);
		}

		for (int i = 0; i < (int)OpponentId::COUNT; i++)
		{
			if (m_avatars[i])
			{
				bool isCurrent = (i == (int)m_opponent);
				m_avatars[i]->SetVisible(isCurrent);
			}
		}
		m_avatar = pAvatar;
	}

	void Game::SetOpponent(OpponentId id)
	{
		m_opponent = id;
		m_brain = AI::Create(id);
		if (m_pFSM) Start();   //new match
	}

	Mood Game::GetAIMood() const
	{
		if (!m_pFSM)
		{
			return Mood::Neutral;
		}

		int state = m_pFSM->state;

		if (state == CPU_ID(StateGoal))
		{
			if (m_lastGoal == Goal::AI)
			{
				return Mood::Happy;
			}
			return Mood::Sad;
		}

		//End
		if (state == CPU_ID(StateGameOver))
		{
			Winner winner = m_match.GetWinner();
			if (winner == Winner::AI)
			{
				return Mood::Happy;
			}
			if (winner == Winner::Player)
			{
				return Mood::Sad;
			}
		}
		return Mood::Neutral;
	}

	bool Game::IsSetup() const { return m_pFSM && m_pFSM->state == CPU_ID(StateSetup); }

	float Game::SetupProgress() const
	{
		if (!IsSetup()) return 1.0f;
		return cpu::Clamp(m_pFSM->totalTime / SETUP_TIME);
	}

	void Game::PointDisplay() {

		XMFLOAT3 IAcolor = CPU_RED;
		XMFLOAT3 PLAYERcolor = CPU_BLUE;

		m_pHUD->NumberTo5(200, 60, m_match.AIScore(), IAcolor);
		m_pHUD->NumberTo5(200, 10, m_match.PlayerScore(), PLAYERcolor);
	}

	void Game::TimerDisplay() {
		
		int dix = (int)(m_match.TimeLeft() * 0.1f);
		int unit = (int)m_match.TimeLeft() % 10;
		XMFLOAT3 color = CPU_WHITE;
		if (dix == 1 || dix == 0)
			color = CPU_RED;
		if (dix == 2)
			color = CPU_ORANGE;
		if (dix == 4 || dix == 3)
			color = { 1.0f, 1.0f, 0.0f };

		m_pHUD->Timer(120, 110, dix, color);
		m_pHUD->Timer(140, 110, unit, color);
	}

	void Game::ShowMenu() {
		if (m_Menu.GetisGameStarted() == false) {
			m_Menu.Update();
			m_pFSM->totalTime = 0.0f;
			m_puck.vel = { 0,0,0 };
			m_ai.vel = { 0,0,0 };
			m_player.vel = { 0,0,0 };
		}
		//totalTime
	}

	void Game::DrawInputMode()
	{
		XMFLOAT3 c = m_input.IsController() ? XMFLOAT3{ 0.2f, 1.0f, 0.2f } : XMFLOAT3{ 0.5f, 0.5f, 0.5f };
		cpuDevice.FillRectangle(8, cpuDevice.GetHeight() - 20, 12, 12, c);
	}

	void Game::DrawVictoryMessage()
	{
		Winner w = m_match.GetWinner();//bippity boppity your line is now my property

		if (w == Winner::AI) {
			m_pHUD->LoseScreen();
		}
		else if (w == Winner::Player) {
			m_pHUD->VictoryScreen();
		}
		else if (w == Winner::Draw) {
			m_pHUD->DrawScreen();
		}
		else { 
			return;
		}
	}
	

	void Game::Stop()
	{
		if (m_Menu.GetisGameStarted() == false){
			ShowMenu();
		}
	}

	void Game::SwitchStopStart()
	{
		if (!m_pFSM)
		{
			return;
		}

		int state = m_pFSM->state;
		if (state == CPU_ID(StatePaused))
		{
			m_pFSM->ToState(m_resumeState);
			m_freeze = false;
		}
		else if (state == CPU_ID(StateServe) || state == CPU_ID(StatePlaying))
		{
			m_resumeState = state;
			m_pFSM->ToState(CPU_ID(StatePaused));
			m_freeze = true;
		}
	}
}
