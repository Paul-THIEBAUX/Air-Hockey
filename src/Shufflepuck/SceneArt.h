#pragma once
class SceneArt
{
public:
	void createPlateau();

	void createDebugOrthonorme();

	//Used by App to move the entities from the game state
	cpu_entity* GetBall() { return m_pBall; }
	cpu_entity* GetPallet() { return m_pPallet; }   //player
	cpu_entity* GetEnnemi() { return m_pEnnemi; }


private:
	// Resources
	cpu_mesh m_meshGoalE;
	cpu_mesh m_meshGoalJ;
	cpu_mesh m_meshBall;
	cpu_mesh m_meshJoueur;
	cpu_mesh m_meshEnnemi;
	cpu_mesh m_meshTable;
	cpu_mesh m_meshLineX;
	cpu_mesh m_meshLineZ;
	cpu_mesh m_meshLineY;
	cpu_mesh m_meshBorder1;
	cpu_mesh m_meshBorder2;
	cpu_font m_font;
	cpu_mesh m_meshCube;
	cpu_mesh m_meshCylinder;
	cpu_rt* m_rts[1];

	// Shader
	cpu_material m_materialGoalE;
	cpu_material m_materialGoalJ;
	cpu_material m_materialPallet; //player
	cpu_material m_materialEnnemi; //Ennemi
	cpu_material m_materialLine; //line
	cpu_material m_materialBall; //ball
	cpu_material m_materialPlate; //plateau
	cpu_material m_materialWall; //wall

	cpu_entity* m_pGoalE = nullptr;
	cpu_entity* m_pGoalJ = nullptr;
	cpu_entity* m_pPallet = nullptr;
	cpu_entity* m_pEnnemi = nullptr;
	cpu_entity* m_pBall = nullptr;
	cpu_entity* m_pTable = nullptr;
	cpu_entity* m_pLineX = nullptr;
	cpu_entity* m_pLineY = nullptr;
	cpu_entity* m_pLineZ = nullptr;
	cpu_entity* m_pBorder1 = nullptr;
	cpu_entity* m_pBorder2 = nullptr;

	cpu_texture m_textureGoalE;
	cpu_texture m_textureGoalJ;
	cpu_texture m_texturePallet;
	cpu_texture m_textureEnnemi;
	cpu_texture m_textureBall;
	cpu_texture m_textureTable;
	cpu_texture m_textureBorder1;
	cpu_texture m_textureBorder2;
	cpu_texture m_textureLineX;
	cpu_texture m_textureLineY;
	cpu_texture m_textureLineZ;

};

