#pragma once
#include "ShufflePhysics.h"

namespace shuffle
{
	enum class Winner { None, Player, AI, Draw };

	// Score / clock / end conditions
	class Match
	{
	public:
		void Reset() { m_player = m_ai = 0; m_timeLeft = MATCH_TIME; m_over = false; m_winner = Winner::None; }
		void Tick(float dt);
		void OnGoal(Goal g);

		int   PlayerScore() const { return m_player; }
		int   AIScore()     const { return m_ai; }
		float TimeLeft()    const { return m_timeLeft; }
		bool  IsOver()      const { return m_over; }
		Winner GetWinner()  const { return m_winner; }

	private:
		void CheckEnd(bool timeUp);
		int m_player = 0;
		int m_ai = 0;
		float m_timeLeft = MATCH_TIME;
		bool m_over = false;
		Winner m_winner = Winner::None;
	};
}
