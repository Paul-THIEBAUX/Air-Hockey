#pragma once
#include "ShuffleConfig.h"
#include "ShufflePhysics.h"

namespace shuffle
{
	//F9 to toggle controller mode
	class PaddleInput
	{
	public:
		void UpdateToggle();   //every frame
		bool GetTarget(const Paddle& self, float dt, XMFLOAT3& out) const;
		bool IsController() const { return m_controller; }
		void SetController(bool on) { m_controller = on; }

	private:
		bool MouseTarget(XMFLOAT3& out) const;
		bool StickTarget(const Paddle& self, float dt, XMFLOAT3& out) const;

		bool m_controller = false;
	};
}
