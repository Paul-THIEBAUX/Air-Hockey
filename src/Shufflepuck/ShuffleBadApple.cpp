#include "pch.h"
#include "ShuffleBadApple.h"
#include "ShuffleData.h"
#include <cstdio>
#include <cmath>
#include <utility>


namespace shuffle
{
	//Same place as the robot
	static const float SCREEN_X = -TABLE_L * 0.5f - 2.0f;
	static const float SCREEN_Y = 0.4f + BADAPPLE_HEIGHT * 0.5f;

	AIBadApple::AIBadApple()
	{
		m_speedFactor = BADAPPLE_SPEED;
		m_reaction = BADAPPLE_REACTION;
		m_turnDelay = BADAPPLE_TURN_DELAY;
		m_error = BADAPPLE_ERROR;
	}

	AIAvatar* AIBadApple::CreateAvatar() const
	{
		return new AvatarBadApple();
	}

	void AvatarBadApple::Build()
	{
		//No file, no video
		//No video, no opponent
		//No opponent, no opponent
		m_path = FindDataFile(BADAPPLE_FILE);
		if (m_path.empty())
		{
			return;
		}
		if (!m_video.Load(m_path.c_str()))
		{
			return;
		}
		m_loaded = true;

		m_gray.assign((size_t)m_video.Width() * (size_t)m_video.Height(), 0);
		m_surface.w = m_video.Width();
		m_surface.h = m_video.Height();

		BuildScreen();
		BuildApple();

		SetVisible(false);
		DebugTitle();
	}

	//Screen
	void AvatarBadApple::BuildScreen()
	{
		const float hw = BADAPPLE_WIDTH * 0.5f;
		const float hh = BADAPPLE_HEIGHT * 0.5f;

		float uLeft = 0.0f;
		float uRight = 1.0f;
		if (BADAPPLE_FLIP_U)
		{
			uLeft = 1.0f;
			uRight = 0.0f;
		}
		//top
		XMFLOAT3 a = { 0.0f, hh, -hw };    //left
		XMFLOAT3 b = { 0.0f, hh, hw };     //right

		//bottom
		XMFLOAT3 c = { 0.0f, -hh, hw };    //right
		XMFLOAT3 d = { 0.0f, -hh, -hw };   //left

		XMFLOAT2 ua = { uLeft, 0.0f };
		XMFLOAT2 ub = { uRight, 0.0f };
		XMFLOAT2 uc = { uRight, 1.0f };
		XMFLOAT2 ud = { uLeft, 1.0f };
		XMFLOAT3 white = { 1.0f, 1.0f, 1.0f };

		m_meshScreen.AddFace(a, b, c, d, ua, ub, uc, ud, white);
		m_meshScreen.AddFace(d, c, b, a, ud, uc, ub, ua, white);
		m_meshScreen.CalculateBoundingVolumes();

		//Pixel shader
		m_materialScreen.lighting = CPU_LIGHTING_UNLIT;
		m_materialScreen.ps = &AvatarBadApple::PixelShader;
		m_materialScreen.values = &m_surface;

		m_pScreen = cpuEngine.CreateEntity();
		m_pScreen->pMesh = &m_meshScreen;
		m_pScreen->pMaterial = &m_materialScreen;

		//Frame behind the screen
		m_meshBezel.CreateCube(1.0f, { 0.10f, 0.10f, 0.13f });
		m_pBezel = cpuEngine.CreateEntity();
		m_pBezel->pMesh = &m_meshBezel;
		m_pBezel->pMaterial = &m_material;
		m_pBezel->transform.sca = { 0.1f, hh + 0.25f, hw + 0.25f };
	}

		//Apple drawn over the puck
	void AvatarBadApple::BuildApple()
	{
		m_meshApple.CreateSphere(0.85f, 8, 10, { 0.85f, 0.08f, 0.08f }, { 0.65f, 0.05f, 0.05f });
		m_meshStem.CreateCylinder(0.2f, 0.05f, 6, true, true, { 0.35f, 0.2f, 0.08f });
		m_meshLeaf.CreateCube(1.0f, { 0.2f, 0.75f, 0.2f });

		m_pApple = cpuEngine.CreateEntity();
		m_pApple->pMesh = &m_meshApple;
		m_pApple->pMaterial = &m_material;

		m_pStem = cpuEngine.CreateEntity();
		m_pStem->pMesh = &m_meshStem;
		m_pStem->pMaterial = &m_material;

		m_pLeaf = cpuEngine.CreateEntity();
		m_pLeaf->pMesh = &m_meshLeaf;
		m_pLeaf->pMaterial = &m_material;
		m_pLeaf->transform.sca = { 0.28f, 0.03f, 0.13f };
	}


	void AvatarBadApple::SetVisible(bool visible)
	{
		if (!m_loaded)
		{
			return;
		}
		m_visible = visible;
		m_dirty = true;
		m_time = 0.0f;
		m_frame = -1;
		if (!visible)
		{
			m_surface.data = nullptr;
		}
	}

	//Void again
	void AvatarBadApple::HideEntity(cpu_entity* pEntity)
	{
		pEntity->transform.SetPosition(0.0f, -1000.0f, 0.0f);
	}


	void AvatarBadApple::Update(float dt, const AvatarInfo& info)
	{
		if (!m_loaded)
		{
			DebugTitle();
			return;
		}

		UpdateScreenPlacement();

		if (m_visible)
		{
			UpdateMapping();
			UpdateVideo(dt);
			UpdateApple(info);
		}
		DebugTitle();
	}

	//Puts the screen and its frame at their place, or hides everything. Only done when something changed (m_dirty).
	void AvatarBadApple::UpdateScreenPlacement()
	{
		if (!m_visible && !m_dirty)
		{
			return;
		}

		if (m_visible)
		{
			m_pScreen->transform.SetPosition(SCREEN_X, SCREEN_Y, 0.0f);
			m_pBezel->transform.SetPosition(SCREEN_X - 0.12f, SCREEN_Y, 0.0f);
		}
		else
		{
			HideEntity(m_pScreen);
			HideEntity(m_pBezel);
			HideEntity(m_pApple);
			HideEntity(m_pStem);
			HideEntity(m_pLeaf);
		}
		m_dirty = false;
	}

	void AvatarBadApple::UpdateVideo(float dt)
	{
		m_time += dt;
		auto frame = (int)(m_time * (float)m_video.Fps());

		bool ended = false;
		if (frame >= m_video.FrameCount())
		{
			if (BADAPPLE_LOOP)
			{
				m_time = 0.0f;
				frame = 0;
			}
			else
			{
				frame = m_video.FrameCount() - 1;
				ended = true;
				m_surface.data = nullptr;   //nothing is drawn: dark screen
			}
		}

		if (ended || frame == m_frame)
		{
			return;
		}
		m_frame = frame;
		if (m_video.Decode(frame, m_gray))
		{
			m_surface.data = m_gray.data();
		}
	}

	void AvatarBadApple::UpdateApple(const AvatarInfo& info)
	{
		m_pApple->transform.SetPosition(info.puck.x, info.puck.y + 0.6f, info.puck.z);
		m_pStem->transform.SetPosition(info.puck.x, info.puck.y + 1.55f, info.puck.z);
		m_pLeaf->transform.SetPosition(info.puck.x + 0.15f, info.puck.y + 1.6f, info.puck.z + 0.12f);
	}


	void AvatarBadApple::UpdateMapping()
	{
		float px[4];
		float py[4];
		if (!ProjectCorners(px, py))
		{
			m_surface.hom[6] = 0.0f;
			m_surface.hom[7] = 0.0f;
			m_surface.data = nullptr;
			return;
		}
		SolveHomography(px, py);
	}

	//Screen corners
	bool AvatarBadApple::ProjectCorners(float px[4], float py[4]) const
	{
		const float hw = BADAPPLE_WIDTH * 0.5f;
		const float hh = BADAPPLE_HEIGHT * 0.5f;
		const float windowW = (float)cpuDevice.GetWidth();
		const float windowH = (float)cpuDevice.GetHeight();

		XMMATRIX world = XMLoadFloat4x4(&m_pScreen->transform.GetWorld());
		XMMATRIX viewProj = XMLoadFloat4x4(&cpuEngine.GetCamera()->matViewProj);
		XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);

		const float cornerY[4] = { hh, hh, -hh, -hh };
		const float cornerZ[4] = { -hw, hw, hw, -hw };

		for (int i = 0; i < 4; i++)
		{
			XMFLOAT4 clip;
			XMStoreFloat4(&clip, XMVector4Transform(XMVectorSet(0.0f, cornerY[i], cornerZ[i], 1.0f), worldViewProj));
			if (clip.w <= 0.0001f)
			{
				return false;
			}
			float ndcX = clip.x / clip.w;   //-1 / +1 (left / right)
			float ndcY = clip.y / clip.w;   //-1 / +1 (bottom / top)
			px[i] = (ndcX * 0.5f + 0.5f) * windowW;
			py[i] = (1.0f - (ndcY * 0.5f + 0.5f)) * windowH;
		}
		return true;
	}

	void AvatarBadApple::SolveHomography(const float px[4], const float py[4])
	{
		float uLeft = 0.0f;
		float uRight = 1.0f;
		if (BADAPPLE_FLIP_U)
		{
			uLeft = 1.0f;
			uRight = 0.0f;
		}
		const float cornerU[4] = { uLeft, uRight, uRight, uLeft };
		const float cornerV[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

		//homography solved
		//a*x + b*y + c - g*x*u - h*y*u = u
		//d*x + e*y + f - g*x*v - h*y*v = v
		//I hate math
		double m[8][9] = {};
		for (int i = 0; i < 4; i++)
		{
			double x = px[i];
			double y = py[i];
			double u = cornerU[i];
			double v = cornerV[i];
			double rowU[9] = { x, y, 1, 0, 0, 0, -x * u, -y * u, u };
			double rowV[9] = { 0, 0, 0, x, y, 1, -x * v, -y * v, v };
			for (int j = 0; j < 9; j++)
			{
				m[2 * i][j] = rowU[j];
				m[2 * i + 1][j] = rowV[j];
			}
		}

		for (int col = 0; col < 8; col++)
		{
			//Pivot
			int pivot = col;
			for (int row = col + 1; row < 8; row++)
			{
				if (fabs(m[row][col]) > fabs(m[pivot][col]))
				{
					pivot = row;
				}
			}
			if (fabs(m[pivot][col]) < 1e-12)
			{
				return;
			}
			for (int j = 0; j < 9; j++)
			{
				std::swap(m[col][j], m[pivot][j]);
			}

			for (int row = 0; row < 8; row++)
			{
				if (row == col)
				{
					continue;
				}
				double factor = m[row][col] / m[col][col];
				for (int j = col; j < 9; j++)
				{
					m[row][j] -= factor * m[col][j];
				}
			}
		}

		//The matrix is now diagonal
		for (int i = 0; i < 8; i++)
		{
			m_surface.hom[i] = (float)(m[i][8] / m[i][i]);
		}
	}

	//Window title
	void AvatarBadApple::DebugTitle()
	{
		if (!BADAPPLE_DEBUG)
		{
			return;
		}

		char text[200];
		if (!m_loaded)
		{
			snprintf(text, sizeof(text), "BadApple: file NOT found (%s): looked in the source folder, the working directory, the exe folder and above", BADAPPLE_FILE);
		}
		else
		{
			const char* state = "off";
			if (m_visible)
			{
				state = "ON";
			}
			snprintf(text, sizeof(text), "BadApple: %dx%d %d frames | %s frame %d | %s", m_video.Width(), m_video.Height(), m_video.FrameCount(), state, m_frame, m_path.c_str());
		}

		if (m_title != text)
		{
			m_title = text;
			SetWindowTextA(cpuWindow.GetHWND(), text);
		}
	}

	void AvatarBadApple::PixelShader(cpu_ps_io& io)
	{
		const Surface* s = (const Surface*)io.values;

		if (s == nullptr || s->data == nullptr)
		{
			io.discard = true;
			return;
		}
		io.discard = false;

		const float* k = s->hom;
		float x = (float)io.p.x;
		float y = (float)io.p.y;
		float divider = k[6] * x + k[7] * y + 1.0f;
		if (divider == 0.0f)
		{
			io.discard = true;
			return;
		}
		float u = (k[0] * x + k[1] * y + k[2]) / divider;
		float v = (k[3] * x + k[4] * y + k[5]) / divider;

		//(u, v) between 0 and 1
		int videoX = cpu::Clamp((int)(u * (float)s->w), 0, s->w - 1);
		int videoY = cpu::Clamp((int)(v * (float)s->h), 0, s->h - 1);

		float gray = (float)s->data[videoY * s->w + videoX] * (1.0f / 255.0f);
		io.color = { gray, gray, gray };
	}
}
