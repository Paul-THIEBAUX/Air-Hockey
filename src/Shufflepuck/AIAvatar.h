#pragma once
#include "ShuffleConfig.h"

namespace shuffle
{
	enum class Mood { Neutral, Happy, Sad };

	struct AvatarInfo
	{
		XMFLOAT3 puck = { 0,0,0 };
		Mood mood = Mood::Neutral;
	};

	class AIAvatar
	{
	public:
		virtual ~AIAvatar() = default;
		virtual void Build() = 0;
		virtual void Update(float dt, const AvatarInfo& info) = 0;
		virtual bool ReplacesPuck() const { return false; } 
		virtual bool IsReady() const { return true; }
		virtual void SetVisible(bool) {}
	};

	class AvatarRobot : public AIAvatar
	{
	public:
		void Build() override;
		void Update(float dt, const AvatarInfo& info) override;
		void SetVisible(bool visible) override;

	private:
		bool m_hidden = false;
		enum Part
		{
			BODY, HEAD, EYE_L, EYE_R, PUPIL_L, PUPIL_R, MOUTH, ANTENNA,
			TIP_NEUTRAL, TIP_HAPPY, TIP_SAD,
			ARM_L, ARM_R,
			COUNT
		};

		struct Piece
		{
			cpu_mesh mesh;
			cpu_entity* pEntity = nullptr;
			XMFLOAT3 offset = { 0,0,0 }; //from origin
			XMFLOAT3 scale = { 1,1,1 }; 
		};

		void Add(int id, XMFLOAT3 offset, XMFLOAT3 scale);
		void SetPose(int id, float dy, float dz, XMFLOAT3 scale);   //position / size of piece
		void SetHeadPose(int id, float extraUp, XMFLOAT3 scale);   //pieces of the head
		void SetArmPose(int id, float angle, float bob);
		void Hide(int id);

		Piece m_pieces[COUNT];
		cpu_material m_material;
		float m_time = 0.0f;        //seconds since start
		float m_headDrop = 0.0f;
		float m_armAngle = 0.1f;
		float m_mouth = 0.5f;       //half width of the mouth
		float m_look = 0.0f;        //how far head looks
		cpu_transform m_headFrame;  //centre of head
	};
}
