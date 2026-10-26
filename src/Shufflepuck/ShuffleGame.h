#pragma once

#include "ShufflePhysics.h"
#include "ShuffleAI.h"
#include "ShuffleMatch.h"
#include "ShufflePaddleInput.h"
#include "HUD.h"
#include "Menu.h"
#include "AIAvatar.h"
#include "ShuffleBadApple.h"
#include <memory>

//Lot may changes when start menu is up

namespace shuffle
{
	class Game
	{
	public:
		struct StateSetup; struct StateServe; struct StatePlaying;
		struct StateGoal;  struct StateGameOver; struct StatePaused; struct StateGlobal;

		void Start();
		void DrawInputMode();   //the bottom left corner, green = controller, grey = mouse
		void UpdateMenu();
		void DrawMenu();
		void DrawVictoryMessage();
		void SwitchStopStart();
		bool IsControllerMode() const { return m_input.IsController(); }
		void SetOpponent(OpponentId id);   //new match against this opponent
		OpponentId GetOpponent() const { return m_opponent; }

		void PointDisplay();
		void TimerDisplay();
		
		//Read-only data for SceneArt / Camera
		const Puck& GetPuck()   const { return m_puck; }
		const Paddle& GetPlayer() const { return m_player; }
		const Paddle& GetAI()     const { return m_ai; }
		const Match& GetMatch()  const { return m_match; }
		std::string   GetStateName() const { return m_pFSM ? m_pFSM->GetName() : std::string(); }
		bool  PuckHidden() const { return m_avatar && m_avatar->ReplacesPuck(); }   //IF the avatar draws its own puck
		bool  IsSetup() const;
		Mood  GetAIMood() const;      //Happy when the AI scored / won, Sad when the player did
		float SetupProgress() const;  // 0..1, drives the intro camera move

	private:
		Menu m_Menu;

		//
		void NewRound(bool puckToPlayer);
		void MovePlayer(float dt);
		void UpdateAvatar();
		void SelectAvatar();
		AIAvatar* GetOrBuildAvatar(OpponentId id);

		cpu_fsm<Game>* m_pFSM = nullptr;
		int m_resumeState = -1;
		bool m_serveToPlayer = true;
		Goal m_lastGoal = Goal::None;
		std::unique_ptr<AIAvatar> m_avatars[(int)OpponentId::COUNT];   //one representation per opponent, built when first used
		AIAvatar* m_avatar = nullptr;                                   //the one of the current opponent
		OpponentId m_opponent = OpponentId::Normal;

		Puck m_puck;
		Paddle m_player;
		Paddle m_ai;
		Physics m_physics;
		std::unique_ptr<AI> m_brain = AI::Create(OpponentId::Normal);
		Match m_match;
		PaddleInput m_input;
	};
}
