#include "pch.h"
#include "ShufflePaddleInput.h"

namespace shuffle
{
	void PaddleInput::UpdateToggle()
	{
		bool keyboard = cpuInput.vi.IsKeyPressed(VK_F9);
		bool pad = cpuInput.xi.IsBackPressed();
		if (keyboard || pad)
		{
			m_controller = !m_controller;
		}
	}

	bool PaddleInput::GetTarget(const Paddle& self, float dt, XMFLOAT3& out) const
	{
		if (m_controller)
		{
			return StickTarget(self, dt, out);
		}
		return MouseTarget(out);
	}

	//target under mouse
	bool PaddleInput::MouseTarget(XMFLOAT3& out) const
	{
		cpu_ray ray;
		cpuEngine.GetCursorRay(ray);

		//Where ray cross table?
		if (fabsf(ray.dir.y) < 1e-6f)
		{
			return false;   //ray parraleel
		}
		float t = (TABLE_Y - ray.pos.y) / ray.dir.y;
		if (t < 0.0f)
		{
			return false;   //behind
		}

		out = cpu::Add3(ray.pos, cpu::Mul3(ray.dir, t));
		out.y = TABLE_Y;
		return true;
	}

	bool PaddleInput::StickTarget(const Paddle& self, float dt, XMFLOAT3& out) const
	{
		//Left stick / dpad
		XMFLOAT2 stick = cpuInput.xi.GetLeftStick();
		if (stick.x == 0.0f && stick.y == 0.0f)
		{
			stick.x = (float)cpuInput.xi.state.x;
			stick.y = (float)cpuInput.xi.state.y;
		}

		//normalize
		float length = sqrtf(stick.x * stick.x + stick.y * stick.y);
		if (length > 1.0f)
		{
			stick.x = stick.x / length;
			stick.y = stick.y / length;
		}

		float step = PADDLE_MAX_SPEED * dt;
		out = self.pos;
		out.x = out.x - stick.y * step;
		out.z = out.z + stick.x * step;
		out.y = TABLE_Y;
		return true; //not move
	}
}
