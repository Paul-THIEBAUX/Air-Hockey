#include "pch.h"
#include "ShufflePhysics.h"

namespace shuffle
{
	static float Len(const XMFLOAT3& v) { return sqrtf(v.x * v.x + v.z * v.z); }

	void Physics::Reset(Puck& puck, float x, XMFLOAT3 vel)
	{
		puck.pos = { x, TABLE_Y, 0.0f };
		puck.vel = vel;
	}

	void Physics::MovePaddle(Paddle& p, XMFLOAT3 target, float dt, float maxSpeed)
	{
		if (dt <= 0.0f)
		{
			p.vel = { 0.0f, 0.0f, 0.0f };
			return;
		}

		const float zMax = TABLE_W * 0.5f - BORDER_R - PADDLE_R;
		target.z = cpu::Clamp(target.z, -zMax, zMax);
		target.x = cpu::Clamp(target.x, p.minX, p.maxX);

		XMFLOAT3 toTarget = cpu::Sub3(target, p.pos);
		toTarget.y = 0.0f;
		float distance = Len(toTarget);
		float maxStep = maxSpeed * dt;
		if (distance > maxStep)
		{
			toTarget = cpu::Mul3(toTarget, maxStep / distance);
		}

		p.pos = cpu::Add3(p.pos, toTarget);
		p.vel = cpu::Mul3(toTarget, 1.0f / dt);
	}

	StepEvents Physics::Step(Puck& puck, const Paddle& player, const Paddle& ai, float dt)
	{
		StepEvents ev;

		//tunneling prevention
		float fastest = Len(puck.vel) + PADDLE_MAX_SPEED;
		auto count = (int)ceilf(fastest * dt / (PUCK_R * 0.5f));
		count = cpu::Clamp(count, 1, 32);
		float subDt = dt / count;

		for (int i = 0; i < count; ++i)
		{
			if (ev.goal != Goal::None)
			{
				break;   //goal end step
			}
			SubStep(puck, player, ai, subDt, ev);
		}
		return ev;
	}

	static void Walls(Puck& puck, StepEvents& ev)
	{
		//Side walls
		const float zMax = TABLE_W * 0.5f - BORDER_R - PUCK_R;
		if (puck.pos.z > zMax)
		{
			puck.pos.z = zMax;
			puck.vel.z = -fabsf(puck.vel.z) * WALL_RESTITUTION;
			ev.wallHit = true;
		}
		else if (puck.pos.z < -zMax)
		{
			puck.pos.z = -zMax;
			puck.vel.z = fabsf(puck.vel.z) * WALL_RESTITUTION;
			ev.wallHit = true;
		}

		//End walls
		bool inFrontOfGoal = fabsf(puck.pos.z) < GOAL_W * 0.5f;
		if (inFrontOfGoal)
		{
			return;
		}
		const float xWall = TABLE_L * 0.5f - BORDER_R - PUCK_R;
		if (puck.pos.x > xWall)
		{
			puck.pos.x = xWall;
			puck.vel.x = -fabsf(puck.vel.x) * WALL_RESTITUTION;
			ev.wallHit = true;
		}
		else if (puck.pos.x < -xWall)
		{
			puck.pos.x = -xWall;
			puck.vel.x = fabsf(puck.vel.x) * WALL_RESTITUTION;
			ev.wallHit = true;
		}
	}

	static void SlideAlongWall(Puck& puck, const Paddle& paddle)
	{
		const float zMax = TABLE_W * 0.5f - BORDER_R - PUCK_R;
		const float xWall = TABLE_L * 0.5f - BORDER_R - PUCK_R;
		const float minDist = PUCK_R + PADDLE_R;

		float dx = puck.pos.x - paddle.pos.x;
		float dz = puck.pos.z - paddle.pos.z;
		bool overlapping = dx * dx + dz * dz < minDist * minDist;
		if (!overlapping)
		{
			return;
		}

		bool onSideWall = fabsf(puck.pos.z) >= zMax - 1e-3f;
		bool onEndWall = fabsf(puck.pos.z) >= GOAL_W * 0.5f && fabsf(puck.pos.x) >= xWall - 1e-3f;

		//Pythagoras...
		if (onSideWall)
		{
			float sign = -1.0f;
			if (dx >= 0.0f)
			{
				sign = 1.0f;
			}
			float rest = fmaxf(0.0f, minDist * minDist - dz * dz);
			puck.pos.x = paddle.pos.x + sign * sqrtf(rest);
		}
		else if (onEndWall)
		{
			float sign = -1.0f;
			if (dz >= 0.0f)
			{
				sign = 1.0f;
			}
			float rest = fmaxf(0.0f, minDist * minDist - dx * dx);
			puck.pos.z = paddle.pos.z + sign * sqrtf(rest);
		}
	}

	void Physics::SubStep(Puck& puck, const Paddle& a, const Paddle& b, float dt, StepEvents& ev)
	{
		//friction
		puck.pos = cpu::Add3(puck.pos, cpu::Mul3(puck.vel, dt));
		puck.vel = cpu::Mul3(puck.vel, fmaxf(0.0f, 1.0f - PUCK_DRAG * dt));

		//puck was able to go out of table
		if (CollidePaddle(puck, a))
		{
			ev.paddleHit = true;
		}
		if (CollidePaddle(puck, b))
		{
			ev.paddleHit = true;
		}
		Walls(puck, ev);

		//Pinned against a wall
		SlideAlongWall(puck, a);
		SlideAlongWall(puck, b);
		Walls(puck, ev);

		//Speed limit
		float speed = Len(puck.vel);
		if (speed > PUCK_MAX_SPEED)
		{
			puck.vel = cpu::Mul3(puck.vel, PUCK_MAX_SPEED / speed);
		}

		//Goal
		const float xEnd = TABLE_L * 0.5f;
		if (puck.pos.x - PUCK_R > xEnd)
		{
			ev.goal = Goal::AI;
		}
		else if (puck.pos.x + PUCK_R < -xEnd)
		{
			ev.goal = Goal::Player;
		}
	}

	bool Physics::CollidePaddle(Puck& puck, const Paddle& p)
	{
		XMFLOAT3 fromPaddle = cpu::Sub3(puck.pos, p.pos);
		fromPaddle.y = 0.0f;
		float distance = Len(fromPaddle);
		float minDistance = PUCK_R + PADDLE_R;
		if (distance >= minDistance)
		{
			return false;   //no touch
		}

		//Normal
		XMFLOAT3 normal;
		if (distance > 1e-5f)
		{
			normal = cpu::Mul3(fromPaddle, 1.0f / distance);
		}
		else
		{
			//Same position
			float towardMiddle = 1.0f;
			if (p.pos.x >= 0.0f)
			{
				towardMiddle = -1.0f;
			}
			normal = { towardMiddle, 0.0f, 0.0f };
		}

		puck.pos.x = p.pos.x + normal.x * minDistance;
		puck.pos.z = p.pos.z + normal.z * minDistance;

		XMFLOAT3 relative = cpu::Sub3(puck.vel, p.vel);
		float approach = cpu::Dot3(relative, normal);
		if (approach < 0.0f)
		{
			float impulse = -(1.0f + PADDLE_RESTITUTION) * approach;
			puck.vel = cpu::Add3(puck.vel, cpu::Mul3(normal, impulse));
		}
		return true;
	}
}
