#pragma once
#include "ShufflePhysics.h"
#include "AIAvatar.h"
#include <memory>

namespace shuffle
{
	//rizturns a target
	//Single behaviour for now (Normal). Other opponents = other classes / parameters later
	enum class OpponentId { Normal, BadApple, COUNT };

	class AI
	{
	public:
		virtual ~AI() = default;
		virtual XMFLOAT3 Think(const Puck& puck, const Paddle& self, float dt);
		virtual AIAvatar* CreateAvatar() const;   //how this AI looks, puck included (the caller owns it)
		virtual const char* Name() const { return "Normal"; }
		virtual void Reset();
		float SpeedFactor() const { return m_state == State::Strike ? 0.90f : m_speedFactor; }

		//New opponent 
		static std::unique_ptr<AI> Create(OpponentId id);

	protected:
		enum class State { Defend, Approach, Strike };

		bool WantsToAttack(const Puck& puck, const Paddle& self, float puckSpeed) const;
		XMFLOAT3 ThinkDefend(const Puck& puck, float dt);
		XMFLOAT3 ThinkAttack(const Puck& puck, const Paddle& self, float puckSpeed);
		void ThinkApproach(const Puck& puck, const Paddle& self, const XMFLOAT3& dir, const XMFLOAT3& behind);
		void StartAttack(const Puck& puck);
		XMFLOAT3 Defend(const Puck& puck);

		State m_state = State::Defend;
		float m_stateTime = 0.0f;
		float m_cooldown = 0.0f;

		float m_speedFactor = 0.65f;      //fraction of PADDLE_MAX_SPEED
		float m_reaction = 0.15f;         //seconds between refreshes
		float m_turnDelay = 0.25f;
		float m_error = 0.6f;             //aiming noise (~AI miss chance)
		float m_timer = 0.0f;
		float m_delay = 0.0f;
		bool m_wasComing = false;
		XMFLOAT3 m_target = { -TABLE_L * 0.25f,TABLE_Y,0 };
		XMFLOAT3 m_aim = { 0,TABLE_Y,0 };   //point the strike is aimed at
		ui32 m_seed = 1234567u;

		//More AI opponent later maybe 
		//if any GOOD ideas come up 
	};
}
