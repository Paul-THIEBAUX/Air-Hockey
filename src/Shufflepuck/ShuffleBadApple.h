#pragma once
#include "BadApple.h"
#include "AIAvatar.h"
#include "ShuffleAI.h"

//Bad Apple opponent
namespace shuffle
{
	constexpr const char* BADAPPLE_FILE = "badapple.bin";
	constexpr bool  BADAPPLE_LOOP = true;
	constexpr bool  BADAPPLE_FLIP_U = false;    //if the video is mirrored

	//screen size (4:3 ratio)
	constexpr float BADAPPLE_WIDTH = 7.0f;
	constexpr float BADAPPLE_HEIGHT = 5.25f;
	constexpr bool  BADAPPLE_DEBUG = true; 

	//Settings of this opponent (more reflex)
	constexpr float BADAPPLE_SPEED = 0.85f;        //fraction of PADDLE_MAX_SPEED (Normal 0.65)
	constexpr float BADAPPLE_REACTION = 0.08f;     //seconds between refreshes (Normal 0.15)
	constexpr float BADAPPLE_TURN_DELAY = 0.10f;   //Normal 0.25
	constexpr float BADAPPLE_ERROR = 0.35f;        //Normal 0.6

	class AIBadApple : public AI
	{
	public:
		AIBadApple();
		AIAvatar* CreateAvatar() const override;
		const char* Name() const override { return "Bad Apple"; }
	};

	class AvatarBadApple : public AIAvatar
	{
	public:
		void Build() override;
		void Update(float dt, const AvatarInfo& info) override;
		void SetVisible(bool visible) override;
		bool IsReady() const override { return m_loaded; }
		bool ReplacesPuck() const override { return m_visible; }

	private:
		//Read by the pixel shader
		struct Surface
		{
			const unsigned char* data = nullptr;   //width*height
			int w = 0, h = 0;
			float hom[8] = { 0,0,0, 0,0,0, 0,0 };   //pixel (x, y) to (u, v)
		};

		static void PixelShader(cpu_ps_io& io);
		void BuildScreen();
		void BuildApple();
		void HideEntity(cpu_entity* pEntity);
		void UpdateScreenPlacement();
		void UpdateVideo(float dt);
		void UpdateApple(const AvatarInfo& info);
		void UpdateMapping();                                       //window pixel to video
		bool ProjectCorners(float px[4], float py[4]) const;
		void SolveHomography(const float px[4], const float py[4]);
		void DebugTitle();

		BadAppleVideo m_video;
		std::vector<unsigned char> m_gray;
		Surface m_surface;
		std::string m_path;
		std::string m_title;
		bool m_loaded = false;
		bool m_visible = false;
		bool m_dirty = true;       //refresh piece pos
		float m_time = 0.0f;
		int m_frame = -1;

		cpu_mesh m_meshScreen, m_meshBezel, m_meshApple, m_meshStem, m_meshLeaf;
		cpu_material m_materialScreen, m_material;
		cpu_entity* m_pScreen = nullptr;
		cpu_entity* m_pBezel = nullptr;
		cpu_entity* m_pApple = nullptr;
		cpu_entity* m_pStem = nullptr;
		cpu_entity* m_pLeaf = nullptr;
	};
}
