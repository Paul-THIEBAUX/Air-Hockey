#include "pch.h"
#include "AIAvatar.h"

namespace shuffle
{
	static const XMFLOAT3 ORIGIN = { -TABLE_L * 0.5f - 2.0f, 0.4f, 0.0f };

	static const float HEAD_Y = 3.7f;
	static const float SHOULDER_Y = 2.4f;
	static const float ARM_HALF_LENGTH = 0.9f;   //half height
	static const float LOOK_DISTANCE = 10.0f;
	static const float LOOK_SIDE_MAX = 6.0f;

	//Moves
	static void Approach(float& value, float target, float dt, float speed)
	{
		value = cpu::Lerp(value, target, fminf(1.0f, dt * speed));
	}

	//Creates entity piece
	void AvatarRobot::Add(int id, XMFLOAT3 offset, XMFLOAT3 scale)
	{
		Piece& p = m_pieces[id];
		p.offset = offset;
		p.scale = scale;
		p.pEntity = cpuEngine.CreateEntity();
		p.pEntity->pMesh = &p.mesh;
		p.pEntity->pMaterial = &m_material;
		SetPose(id, 0.0f, 0.0f, scale);
	}

	//send to the void
	void AvatarRobot::Hide(int id)
	{
		Piece& p = m_pieces[id];
		if (p.pEntity == nullptr) return;
		p.pEntity->transform.SetPosition(0.0f, -1000.0f, 0.0f);
	}

	//Moves / resizes
	void AvatarRobot::SetPose(int id, float dy, float dz, XMFLOAT3 scale)
	{
		Piece& p = m_pieces[id];
		if (p.pEntity == nullptr) return;

		p.pEntity->transform.sca = scale;
		if (m_hidden)
		{
			Hide(id);
			return;
		}

		float x = ORIGIN.x + p.offset.x;
		float y = ORIGIN.y + p.offset.y + dy;
		float z = ORIGIN.z + p.offset.z + dz;
		p.pEntity->transform.SetPosition(x, y, z);
	}

	void AvatarRobot::SetHeadPose(int id, float extraUp, XMFLOAT3 scale)
	{
		Piece& p = m_pieces[id];
		if (p.pEntity == nullptr) return;

		p.pEntity->transform.sca = scale;
		if (m_hidden)
		{
			Hide(id);
			return;
		}

		//Offset
		float forward = p.offset.x;
		float up = p.offset.y - HEAD_Y + extraUp;
		float side = -p.offset.z;

		XMFLOAT3 pos = m_headFrame.pos;
		pos.x = pos.x + forward * m_headFrame.dir.x + up * m_headFrame.up.x + side * m_headFrame.right.x;
		pos.y = pos.y + forward * m_headFrame.dir.y + up * m_headFrame.up.y + side * m_headFrame.right.y;
		pos.z = pos.z + forward * m_headFrame.dir.z + up * m_headFrame.up.z + side * m_headFrame.right.z;

		p.pEntity->transform.SetPosition(pos.x, pos.y, pos.z);
		p.pEntity->transform.SetRotation(m_headFrame);   //turns with the head
	}


	void AvatarRobot::SetArmPose(int id, float angle, float bob)
	{
		Piece& p = m_pieces[id];
		if (p.pEntity == nullptr) return;
		if (m_hidden)
		{
			Hide(id);
			return;
		}

		float outward = 1.0f;
		if (p.offset.z < 0.0f)
		{
			outward = -1.0f;
		}

		XMFLOAT3 arm = { 0.0f, -cosf(angle), outward * sinf(angle) };


		float x = ORIGIN.x + p.offset.x + arm.x * ARM_HALF_LENGTH;
		float y = ORIGIN.y + SHOULDER_Y + bob + arm.y * ARM_HALF_LENGTH;
		float z = ORIGIN.z + p.offset.z + arm.z * ARM_HALF_LENGTH;

		p.pEntity->transform.sca = { 1.0f, 1.0f, 1.0f };
		p.pEntity->transform.SetPosition(x, y, z);
		p.pEntity->transform.LookTo(1.0f, 0.0f, 0.0f, arm);
	}

	//Opponent change
	void AvatarRobot::SetVisible(bool visible)
	{
		m_hidden = !visible;
		if (m_hidden)
		{
			for (int id = 0; id < COUNT; id++)
				Hide(id);
		}
	}

	//Build mesh,places pieces
	void AvatarRobot::Build()
	{
		const XMFLOAT3 steel = { 0.55f, 0.60f, 0.70f };
		const XMFLOAT3 dark = { 0.12f, 0.12f, 0.16f };
		const XMFLOAT3 white = { 1.0f, 1.0f, 1.0f };

		//Body
		m_pieces[BODY].mesh.CreateCube(1.0f, steel);
		Add(BODY, { 0.0f, 1.3f, 0.0f }, { 0.9f, 1.3f, 1.7f });

		m_pieces[HEAD].mesh.CreateSphere(1.2f, 8, 10, steel, steel);
		Add(HEAD, { 0.0f, 3.7f, 0.0f }, { 1.0f, 1.0f, 1.0f });

		//Head / eyes
		m_pieces[EYE_L].mesh.CreateSphere(0.3f, 6, 8, white, white);
		m_pieces[EYE_R].mesh.CreateSphere(0.3f, 6, 8, white, white);
		Add(EYE_L, { 1.05f, 3.9f, -0.55f }, { 1,1,1 });
		Add(EYE_R, { 1.05f, 3.9f, 0.55f }, { 1,1,1 });

		m_pieces[PUPIL_L].mesh.CreateSphere(0.15f, 5, 6, dark, dark);
		m_pieces[PUPIL_R].mesh.CreateSphere(0.15f, 5, 6, dark, dark);
		Add(PUPIL_L, { 1.27f, 3.9f, -0.55f }, { 1,1,1 });
		Add(PUPIL_R, { 1.27f, 3.9f, 0.55f }, { 1,1,1 });

		//Mouth
		m_pieces[MOUTH].mesh.CreateCube(1.0f, dark);
		Add(MOUTH, { 1.1f, 3.2f, 0.0f }, { 0.5f, 0.07f, 0.08f });

		//Antenna / light
		m_pieces[ANTENNA].mesh.CreateCylinder(0.45f, 0.07f, 6, true, true, dark);
		Add(ANTENNA, { 0.0f, 5.35f, 0.0f }, { 1,1,1 });

		const XMFLOAT3 cyan = { 0.2f, 0.9f, 1.0f }, yellow = { 1.0f, 0.9f, 0.1f }, red = { 1.0f, 0.15f, 0.15f };
		m_pieces[TIP_NEUTRAL].mesh.CreateSphere(0.25f, 6, 8, cyan, cyan);
		m_pieces[TIP_HAPPY].mesh.CreateSphere(0.25f, 6, 8, yellow, yellow);
		m_pieces[TIP_SAD].mesh.CreateSphere(0.25f, 6, 8, red, red);
		Add(TIP_NEUTRAL, { 0.0f, 5.95f, 0.0f }, { 1,1,1 });
		Add(TIP_HAPPY, { 0.0f, 5.95f, 0.0f }, { 1,1,1 });
		Add(TIP_SAD, { 0.0f, 5.95f, 0.0f }, { 1,1,1 });

		//Arms
		m_pieces[ARM_L].mesh.CreateCylinder(0.9f, 0.28f, 8, true, true, steel);
		m_pieces[ARM_R].mesh.CreateCylinder(0.9f, 0.28f, 8, true, true, steel);
		Add(ARM_L, { 0.0f, 1.5f, -2.1f }, { 1,1,1 });
		Add(ARM_R, { 0.0f, 1.5f, 2.1f }, { 1,1,1 });
	}

	//Choose target pose from the mood / move / place
	void AvatarRobot::Update(float dt, const AvatarInfo& info)
	{
		if (!m_pieces[BODY].pEntity) return;
		m_time += dt;

		//Target pose
		float bob;
		float headDrop = 0.0f;
		float armAngle = 0.1f;
		float mouth = 0.5f;
		int tip = TIP_NEUTRAL;
		switch (info.mood)
		{
		case Mood::Happy:
			bob = fabsf(sinf(m_time * 9.0f)) * 0.45f;
			armAngle = 2.5f + 0.4f * sinf(m_time * 12.0f);
			mouth = 0.9f;
			tip = TIP_HAPPY;
			break;

		case Mood::Sad:
			bob = sinf(m_time * 1.2f) * 0.03f;
			headDrop = -0.35f;
			armAngle = 0.0f;
			mouth = 0.3f;
			tip = TIP_SAD;
			break;

		default:
			bob = sinf(m_time * 2.5f) * 0.08f; 
			break;
		}
		Approach(m_headDrop, headDrop, dt, 6.0f);
		Approach(m_armAngle, armAngle, dt, 6.0f);
		Approach(m_mouth, mouth, dt, 8.0f);
		//head turn to puck
		float lookTarget = cpu::Clamp(info.puck.z / (TABLE_W * 0.5f), -1.0f, 1.0f) * LOOK_SIDE_MAX;
		Approach(m_look, lookTarget, dt, 10.0f);

		const XMFLOAT3 one = { 1.0f, 1.0f, 1.0f };
		SetPose(BODY, bob, 0.0f, m_pieces[BODY].scale);

		//head frame
		const float head = bob + m_headDrop;
		m_headFrame.pos = { ORIGIN.x, ORIGIN.y + HEAD_Y + head, ORIGIN.z };
		m_headFrame.LookAt(m_headFrame.pos.x + LOOK_DISTANCE, m_headFrame.pos.y, m_headFrame.pos.z + m_look);

		SetHeadPose(HEAD, 0.0f, one);
		SetHeadPose(EYE_L, 0.0f, one);
		SetHeadPose(EYE_R, 0.0f, one);
		SetHeadPose(ANTENNA, 0.0f, one);
		SetHeadPose(PUPIL_L, 0.0f, one);
		SetHeadPose(PUPIL_R, 0.0f, one);

		//Mouth
		float mouthDrop = 0.0f;
		if (m_mouth < 0.4f)
		{
			mouthDrop = -0.1f;
		}
		SetHeadPose(MOUTH, mouthDrop, { m_mouth, 0.07f, 0.08f });

		//Antenna light
		float pulseSpeed = 4.0f;
		if (info.mood == Mood::Happy)
		{
			pulseSpeed = 14.0f;
		}
		const float pulse = 1.0f + 0.25f * sinf(m_time * pulseSpeed);
		const XMFLOAT3 pulseScale = { pulse, pulse, pulse };

		Hide(TIP_NEUTRAL);
		Hide(TIP_HAPPY);
		Hide(TIP_SAD);
		SetHeadPose(tip, 0.0f, pulseScale);

		//Arms swing
		SetArmPose(ARM_L, m_armAngle, bob);
		SetArmPose(ARM_R, m_armAngle, bob);
	}
}
