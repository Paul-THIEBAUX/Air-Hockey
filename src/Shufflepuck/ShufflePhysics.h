#pragma once
#include "ShuffleConfig.h"

namespace shuffle
{
	struct Paddle
	{
		XMFLOAT3 pos = { 0,TABLE_Y,0 };
		XMFLOAT3 vel = { 0,0,0 };   //drives hit impulse
		float minX = 0;
		float maxX = 0; //half of table
	};

	struct Puck
	{
		XMFLOAT3 pos = { 0,TABLE_Y,0 };
		XMFLOAT3 vel = { 0,0,0 };
	};

	enum class Goal { None, Player, AI };   // who scored

	struct StepEvents
	{
		bool wallHit = false;
		bool paddleHit = false;
		Goal goal = Goal::None;
	};

	//simulation
	class Physics
	{
	public:
		void Reset(Puck& puck, float x, XMFLOAT3 vel);
		StepEvents Step(Puck& puck, const Paddle& player, const Paddle& ai, float dt);
		static void MovePaddle(Paddle& p, XMFLOAT3 target, float dt, float maxSpeed = PADDLE_MAX_SPEED);

	private:
		void SubStep(Puck& puck, const Paddle& a, const Paddle& b, float dt, StepEvents& ev);
		static bool CollidePaddle(Puck& puck, const Paddle& p);
	};
}