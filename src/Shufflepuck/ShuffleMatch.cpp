#include "pch.h"
#include "ShuffleMatch.h"

namespace shuffle
{
	void Match::Tick(float dt)
	{
		if (m_over) return;
		m_timeLeft -= dt;
		if (m_timeLeft <= 0.0f) { m_timeLeft = 0.0f; CheckEnd(true); }
	}

	void Match::OnGoal(Goal g)
	{
		if (m_over) return;
		if (g == Goal::Player) ++m_player;
		else if (g == Goal::AI) ++m_ai;
		CheckEnd(false);
	}

	void Match::CheckEnd(bool timeUp)
	{
		if (m_player >= SCORE_LIMIT || m_ai >= SCORE_LIMIT || timeUp)
		{
			m_over = true;
			if (m_player > m_ai)
			{
				m_winner = Winner::Player;
			}
			else if (m_ai > m_player)
			{
				m_winner = Winner::AI;
			}
			else
			{
				m_winner = Winner::Draw;
			}
		}
	}
}
