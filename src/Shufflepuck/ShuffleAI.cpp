#include "pch.h"
#include "ShuffleAI.h"
#include "ShuffleBadApple.h"

namespace shuffle
{

	std::unique_ptr<AI> AI::Create(OpponentId id)
	{
		switch (id)
		{
		case OpponentId::BadApple: return std::unique_ptr<AI>(new AIBadApple());
		default:                   return std::unique_ptr<AI>(new AI());
		}
	}

	AIAvatar* AI::CreateAvatar() const
	{
		return new AvatarRobot();
	}

	static const float MAX_PUCK_SPEED_TO_ATTACK = 8.0f;
	static const float ABORT_PUCK_SPEED = 11.0f;
	static const float APPROACH_TIME_LIMIT = 1.5f;
	static const float STRIKE_TIME_LIMIT = 0.7f;
	static const float PUCK_GOING_BACK_SPEED = 2.0f;
	static const float COOLDOWN_AFTER_LOST = 0.3f;
	static const float COOLDOWN_AFTER_STRIKE = 0.8f;
	static const float BEHIND_MARGIN = 0.4f;
	static const float STRIKE_START_DISTANCE = 0.7f;
	static const float DETOUR_DISTANCE = 2.2f;
	static const float STRIKE_REACH = 1.5f;
	static const float BANK_SHOT_CHANCE = 0.3f;
	static const float COMING_SPEED = -0.5f;

	XMFLOAT3 AI::Think(const Puck& puck, const Paddle& self, float dt)
	{
		m_stateTime += dt;
		m_cooldown -= dt;

		float puckSpeed = sqrtf(puck.vel.x * puck.vel.x + puck.vel.z * puck.vel.z);

		if (m_state == State::Defend)
		{
			if (WantsToAttack(puck, self, puckSpeed))
			{
				StartAttack(puck);
			}
			else
			{
				return ThinkDefend(puck, dt);
			}
		}
		return ThinkAttack(puck, self, puckSpeed);
	}

	//Attack
	bool AI::WantsToAttack(const Puck& puck, const Paddle& self, float puckSpeed) const
	{
		const float sum = PUCK_R + PADDLE_R;
		bool onOurHalf = puck.pos.x < -1.0f;
		bool inReach = puck.pos.x > self.minX + sum;
		bool slowEnough = puckSpeed < MAX_PUCK_SPEED_TO_ATTACK;
		return m_cooldown <= 0.0f && onOurHalf && inReach && slowEnough;
	}

	//Defending
	XMFLOAT3 AI::ThinkDefend(const Puck& puck, float dt)
	{
		bool coming = puck.vel.x < COMING_SPEED;
		if (coming && !m_wasComing)
		{
			m_delay = m_turnDelay;
		}
		m_wasComing = coming;

		m_delay -= dt;
		m_timer -= dt;

		//Refresh
		if (m_timer <= 0.0f && m_delay <= 0.0f)
		{
			m_timer = m_reaction;
			m_target = Defend(puck);
		}
		return m_target;
	}

	//Approach and strike
	XMFLOAT3 AI::ThinkAttack(const Puck& puck, const Paddle& self, float puckSpeed)
	{
		const float sum = PUCK_R + PADDLE_R;

		XMFLOAT3 dir = cpu::Sub3(m_aim, puck.pos);
		dir.y = 0.0f;
		float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
		if (len > 1e-4f)
		{
			dir = cpu::Mul3(dir, 1.0f / len);
		}
		else
		{
			dir = { 1.0f, 0.0f, 0.0f };
		}


		XMFLOAT3 behind = cpu::Sub3(puck.pos, cpu::Mul3(dir, sum + BEHIND_MARGIN));

		//Stop?
		bool puckLost = puck.pos.x > 0.0f || puck.pos.x < self.minX + sum || puckSpeed > ABORT_PUCK_SPEED;
		bool approachTooLong = m_state == State::Approach && m_stateTime > APPROACH_TIME_LIMIT;
		bool strikeTooLong = m_state == State::Strike && m_stateTime > STRIKE_TIME_LIMIT;
		bool strikeDone = m_state == State::Strike && puck.vel.x > PUCK_GOING_BACK_SPEED;
		if (puckLost || approachTooLong || strikeTooLong || strikeDone)
		{
			m_state = State::Defend;
			m_stateTime = 0.0f;
			if (puckLost)
			{
				m_cooldown = COOLDOWN_AFTER_LOST;
			}
			else
			{
				m_cooldown = COOLDOWN_AFTER_STRIKE;
			}
			m_delay = 0.0f;
			m_timer = 0.0f;
			m_target = Defend(puck);
			return m_target;
		}

		if (m_state == State::Approach)
		{
			ThinkApproach(puck, self, dir, behind);
		}
		else
		{
			//Strike
			m_target = cpu::Add3(puck.pos, cpu::Mul3(dir, STRIKE_REACH));
		}
		m_target.y = TABLE_Y;
		return m_target;
	}

	void AI::ThinkApproach(const Puck& puck, const Paddle& self, const XMFLOAT3& dir, const XMFLOAT3& behind)
	{
		const float sum = PUCK_R + PADDLE_R;
		XMFLOAT3 toSelf = cpu::Sub3(self.pos, puck.pos);
		bool isBehind = cpu::Dot3(toSelf, dir) < -sum;

		if (!isBehind)
		{
			float side = -1.0f;
			if (self.pos.z >= puck.pos.z)
			{
				side = 1.0f;
			}
			XMFLOAT3 perpendicular = { -dir.z, 0.0f, dir.x };
			m_target = cpu::Add3(behind, cpu::Mul3(perpendicular, side * DETOUR_DISTANCE));
			return;
		}

		m_target = behind;
		XMFLOAT3 toSpot = cpu::Sub3(behind, self.pos);
		float distSq = toSpot.x * toSpot.x + toSpot.z * toSpot.z;
		if (distSq < STRIKE_START_DISTANCE * STRIKE_START_DISTANCE)
		{
			m_state = State::Strike;
			m_stateTime = 0.0f;
		}
	}

	void AI::StartAttack(const Puck& puck)
	{
		(void)puck;
		m_state = State::Approach;
		m_stateTime = 0.0f;

		//Direct shot
		const float zWall = TABLE_W * 0.5f - BORDER_R - PUCK_R;
		float goalZ = cpu::RandSigned(m_seed) * (GOAL_W * 0.5f - PUCK_R);
		m_aim = { TABLE_L * 0.5f, TABLE_Y, goalZ };

		//bank shot
		if (cpu::Rand01(m_seed) < BANK_SHOT_CHANCE)
		{
			float wall = -zWall;
			if (cpu::RandSigned(m_seed) >= 0.0f)
			{
				wall = zWall;
			}
			m_aim.z = 2.0f * wall - goalZ;
		}
	}

	XMFLOAT3 AI::Defend(const Puck& puck)
	{
		const float zWall = TABLE_W * 0.5f - BORDER_R - PUCK_R;
		const float defendX = -TABLE_L * 0.25f;
		XMFLOAT3 target = { defendX, TABLE_Y, 0.0f };

		if (puck.vel.x < COMING_SPEED)
		{
			float timeToLine = fmaxf(0.0f, (defendX - puck.pos.x) / puck.vel.x);
			target.z = puck.pos.z + puck.vel.z * timeToLine;
			if (puck.pos.x < defendX)
			{
				target.z = puck.pos.z;
			}
		}
		else
		{
			target.z = puck.pos.z * 0.5f;
		}

		//The error makes the AI beatable
		//In theory
		target.z = target.z + cpu::RandSigned(m_seed) * m_error;
		target.z = cpu::Clamp(target.z, -zWall, zWall);
		return target;
	}
}
