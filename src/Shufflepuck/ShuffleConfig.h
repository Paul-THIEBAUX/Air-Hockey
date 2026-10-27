#pragma once

//values to chnage / move according to the table and puck sizes, etc.

namespace shuffle
{
	//W and L was inverted
	constexpr float TABLE_W = 10.0f;   //along Z
	constexpr float TABLE_L = 20.0f;   //along X
	constexpr float TABLE_Y = 0.0f;    //height of the playing surface
	constexpr float PUCK_R = 0.75f;    //same as m_meshBall radius
	constexpr float PADDLE_R = 1.0f;    //same as mesh radius player/AI
	constexpr float BORDER_R = 0.25f;    //radius of  order cylinders
	constexpr float GOAL_W = 5.0f;       //goal opening
	constexpr float PUCK_MAX_SPEED = 18.0f;
	constexpr float PADDLE_MAX_SPEED = 14.0f;
	constexpr float WALL_RESTITUTION = 0.92f;
	constexpr float PADDLE_RESTITUTION = 0.85f;
	constexpr float PUCK_DRAG = 0.15f;   //speed lost per second

	constexpr float MATCH_TIME = 90.0f;
	constexpr int   SCORE_LIMIT = 5;
	constexpr float SERVE_DELAY = 1.0f;
	constexpr float SETUP_TIME = 3.0f;
	constexpr float GOAL_PAUSE = 1.2f;
}
