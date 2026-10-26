#include "pch.h"
#include "SceneArt.h"
#include "Camera.h"

void SceneArt::createPlateau()
{
	float dt = cpuTime.delta;
	
	m_meshTable.CreateCube(5.0f, CPU_WHITE);
	m_pTable = cpuEngine.CreateEntity();

	m_pTable->pMesh = &m_meshTable;
	m_pTable->pMaterial = &m_materialPlate;
	m_pTable->transform.pos.x = 5.0f; //red
	m_pTable->transform.pos.y = -5.0f; //green
	m_pTable->transform.pos.z = 0.0f; //blue

	m_meshTable.CreateCube(5.0f, CPU_WHITE);
	m_pTable = cpuEngine.CreateEntity();

	m_pTable->pMesh = &m_meshTable;
	m_pTable->pMaterial = &m_materialPlate;
	m_pTable->transform.pos.x = -5.0f; //red
	m_pTable->transform.pos.y = -5.0f; //green
	m_pTable->transform.pos.z = 0.0f; //blue

	XMFLOAT3 RGBBorder = {0.173f, 0.035f, 0.554f};

	/*left border*/
	m_meshBorder1.CreateCylinder(10.0f, 0.25f, 6, true, true, RGBBorder);
	m_pBorder1 = cpuEngine.CreateEntity();
	m_pBorder1->pMesh = &m_meshBorder1;
	m_pBorder1->pMaterial = &m_materialLine;
	m_pBorder1->transform.pos.x = 0.0f;
	m_pBorder1->transform.pos.y = 0.0f;
	m_pBorder1->transform.pos.z = -5.0f;
	m_pBorder1->transform.AddYPR(0.0f, 0.0f, XM_PI * 0.5f);

	/*right border*/
	m_meshBorder1.CreateCylinder(10.0f, 0.25f, 6, true, true, RGBBorder);
	m_pBorder1 = cpuEngine.CreateEntity();
	m_pBorder1->pMesh = &m_meshBorder1;
	m_pBorder1->pMaterial = &m_materialLine;
	m_pBorder1->transform.pos.x = 0.0f;
	m_pBorder1->transform.pos.y = 0.0f;
	m_pBorder1->transform.pos.z = 5.0f;
	m_pBorder1->transform.AddYPR(0.0f, 0.0f, XM_PI * 0.5f);

	/*bottom border*/
	m_meshBorder2.CreateCylinder(5.0f, 0.25f, 6, true, true, RGBBorder);
	m_pBorder2 = cpuEngine.CreateEntity();
	m_pBorder2->pMesh = &m_meshBorder2;
	m_pBorder2->pMaterial = &m_materialLine;
	m_pBorder2->transform.pos.x = 10.0f;
	m_pBorder2->transform.pos.y = 0.0f;
	m_pBorder2->transform.pos.z = 0.0f;
	m_pBorder2->transform.AddYPR(0.0f, XM_PI * 0.5f, 0.0f);

	/*upper border*/
	m_meshBorder2.CreateCylinder(5.0f, 0.25f, 6, true, true, RGBBorder);
	m_pBorder2 = cpuEngine.CreateEntity();
	m_pBorder2->pMesh = &m_meshBorder2;
	m_pBorder2->pMaterial = &m_materialLine;
	m_pBorder2->transform.pos.x = -10.0f;
	m_pBorder2->transform.pos.y = 0.0f;
	m_pBorder2->transform.pos.z = 0.0f;
	m_pBorder2->transform.AddYPR(0.0f, XM_PI * 0.5f, 0.0f);

	/*bottom Goal*/
	m_meshGoalJ.CreateCylinder(2.5f, 0.30f, 6, true, true, CPU_BLACK);
	m_pGoalJ = cpuEngine.CreateEntity();
	m_pGoalJ->pMesh = &m_meshGoalJ;
	m_pGoalJ->pMaterial = &m_materialLine;
	m_pGoalJ->transform.pos.x = 10.0f;
	m_pGoalJ->transform.pos.y = 0.0f;
	m_pGoalJ->transform.pos.z = 0.0f;
	m_pGoalJ->transform.AddYPR(0.0f, XM_PI * 0.5f, 0.0f);

	/*upper Goal*/
	m_meshGoalE.CreateCylinder(2.5f, 0.30f, 6, true, true, CPU_BLACK);
	m_pGoalE = cpuEngine.CreateEntity();
	m_pGoalE->pMesh = &m_meshGoalE;
	m_pGoalE->pMaterial = &m_materialLine;
	m_pGoalE->transform.pos.x = -10.0f;
	m_pGoalE->transform.pos.y = 0.0f;
	m_pGoalE->transform.pos.z = 0.0f;
	m_pGoalE->transform.AddYPR(0.0f, XM_PI * 0.5f, 0.0f);

	m_meshJoueur.CreateCylinder(0.25f, 1.0f, 16, true, true, CPU_BLUE);
	m_pPallet = cpuEngine.CreateEntity();
	m_pPallet->pMesh = &m_meshJoueur;
	m_pPallet->pMaterial = &m_materialLine;
	m_pPallet->transform.pos.x = 5.0f;
	m_pPallet->transform.pos.y = 0.0f;
	m_pPallet->transform.pos.z = 0.0f;

	m_meshEnnemi.CreateCylinder(0.25f, 1.0f, 16, true, true, CPU_RED);
	m_pEnnemi = cpuEngine.CreateEntity();
	m_pEnnemi->pMesh = &m_meshEnnemi;
	m_pEnnemi->pMaterial = &m_materialLine;
	m_pEnnemi->transform.pos.x = -5.0f;
	m_pEnnemi->transform.pos.y = 0.0f;
	m_pEnnemi->transform.pos.z = 0.0f;

	m_meshBall.CreateCylinder(0.25f, 0.75f, 16, true, true, CPU_GREEN);
	m_pBall = cpuEngine.CreateEntity();
	m_pBall->pMesh = &m_meshBall;
	m_pBall->pMaterial = &m_materialLine;
	m_pBall->transform.pos.x = 0.0f;
	m_pBall->transform.pos.y = 0.0f;
	m_pBall->transform.pos.z = 0.0f;


}

void SceneArt::createDebugOrthonorme()
{
	float dt = cpuTime.delta;

	//CreateCylinder(float halfHeight = 0.5f, float radius = 0.5f, int count = 6, bool top = true, bool bottom = true, XMFLOAT3 color = CPU_WHITE);
	//CreateCube(float halfSize = 0.5f, XMFLOAT3 color = CPU_WHITE);
	m_meshLineX.CreateCylinder(100.0f, 0.05f, 6,false,false, CPU_GREEN);
	m_pLineX = cpuEngine.CreateEntity();
	m_pLineX->pMesh = &m_meshLineX;
	m_pLineX->pMaterial = &m_materialLine;
	m_pLineX->transform.pos.x = 0.0f;
	m_pLineX->transform.pos.y = 0.0f;
	m_pLineX->transform.pos.z = 0.0f;

	m_meshLineY.CreateCylinder(100.0f, 0.05f, 6, false, false, CPU_RED);
	m_pLineY = cpuEngine.CreateEntity();
	m_pLineY->pMesh = &m_meshLineY;
	m_pLineY->pMaterial = &m_materialLine;
	m_pLineY->transform.pos.x = 0.0f;
	m_pLineY->transform.pos.y = 0.0f;
	m_pLineY->transform.pos.z = 0.0f;
	m_pLineY->transform.AddYPR(0.0f, 0.0f, XM_PI * 0.5f);

	m_meshLineZ.CreateCylinder(100.0f, 0.05f, 6, false, false, CPU_BLUE);
	m_pLineZ = cpuEngine.CreateEntity();
	m_pLineZ->pMesh = &m_meshLineZ;
	m_pLineZ->pMaterial = &m_materialLine;
	m_pLineZ->transform.pos.x = 0.0f;
	m_pLineZ->transform.pos.y = 0.0f;
	m_pLineZ->transform.pos.z = 0.0f;
	m_pLineZ->transform.AddYPR(0.0f, XM_PI * 0.5f, 0.0f);
}