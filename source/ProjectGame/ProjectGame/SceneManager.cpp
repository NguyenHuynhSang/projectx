﻿#include "SceneManager.h"
#include"Camera.h"
#include"Stair.h"
#include"HiddenObjects.h"
#include"Effect.h"
#include"Panther.h"
#include"Game.h"
#include"Door.h"
#include"Bat.h"
#include"Fishmen.h"
#include"Ground.h"
#include"GameUI.h"
CSceneManager * CSceneManager::__instance = NULL;

CSceneManager * CSceneManager::GetInstance()
{
	if (__instance == NULL) __instance = new CSceneManager();
	return __instance;
}


void CSceneManager::LoadResource()
{
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_BBOX, L"Resource\\sprites\\bbox.png", D3DCOLOR_XRGB(201, 191, 231));
	textures->Add(ID_BACKGROUND_LV1, L"data\\map\\lv1.BMP", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_BACKGROUND_LV2, L"data\\map\\lv1_scene1.BMP", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_BACKGROUND_LV3, L"data\\map\\lv1_scene2.BMP", D3DCOLOR_XRGB(255, 255, 255));
	CGameUI* ui = CGameUI::GetInstance();
	ui->LoadResource();

	CItem *items = new CItem();
	items->LoadResource();

	CDagger *dagger = new CDagger();
	dagger->LoadResource();

	CBrick *brick = new CBrick();
	brick->LoadResource();

	CGhoul *ghoul = new CGhoul();
	ghoul->LoadResource();

	CPanther *panther = new CPanther();
	panther->LoadResource();

	CBat *bat = new CBat();
	bat->LoadResource();

	CFishmen *fishman = new CFishmen();
	
	fishman->LoadResource();

	CTorch *torch = new CTorch();
	torch->LoadResource();

	CStair *stair = new CStair();
	stair->LoadResource();

	

	CEffect *eff = new CEffect();
	eff->LoadResource();

	CDoor *door = new CDoor();
	door->LoadResource();

}

void CSceneManager::LoadMap()
{
	switch (currentScene)
	{
	case SCENE_STATE_FIRST: 
	{
	
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_BACKGROUND_LV1); //tex Map
		tileG = new CTileMap();
		tileG->SetMSize(1568, 352);
		tileG->SetTileSetHeight(640, 224);
		tileG->LoadTile(MAP_MATRIXPATH_SCENE1, texMap);

		break;
	}
	case SCENE_STATE_SECOND:
	{
	
	}
	case SCENE_STATE_THIRD:
	{
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_BACKGROUND_LV2); //tex Map
		tileG = new CTileMap();
		tileG->SetMSize(5664, 352);
		tileG->SetTileSetHeight(640, 160);
		tileG->LoadTile(MAP_MATRIXPATH_SCENE2, texMap);

		break;

	}
	case SCENE_STATE_FOURTH:
	{
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_BACKGROUND_LV3); //tex Map
		tileG = new CTileMap();
		tileG->SetMSize(1056, 352);
		tileG->SetTileSetHeight(640, 96);
		tileG->LoadTile(MAP_MATRIXPATH_SCENE3, texMap);
		break;

	}


	}
}

void CSceneManager::RenderMap()
{
	tileG->DrawMap();
}

void CSceneManager::initScene()
{

	switch (currentScene)
	{
	case SCENE_STATE_FIRST:
	{
		gameState = 1;
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		simon->SetPosition(100, 375-64);
		CGlobal::GetInstance()->objects.push_back(simon);

		CGround *ground;
		
		CDoor* door = new CDoor();
		door->SetPosition(1408, 87);
		door->SetState(DOOR_STATE_BIGDOOR);
		CGlobal::GetInstance()->objects.push_back(door);
		
		ground = new CGround();
		ground->setBoundBox(1568, 32);
		ground->SetPosition(0, 375);
		CGlobal::GetInstance()->objects.push_back(ground);
		for (int i = 0; i < 1536 / 32 + 4; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetPosition(i * 32, 350 + 25);
			//CGlobal::GetInstance()->objects.push_back(brick);
		}

		CTorch* Torch = new CTorch();
		Torch->SetPosition(0 + 450, 350 - 64 + 25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_MHEART);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();
		Torch->SetPosition(750, 350 - 64 + 25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_NWHIP);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();
		Torch->SetPosition(950, 350 - 64 + 25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_NWHIP);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();;
		Torch->SetPosition(1200, 350 - 64 + 25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_DANGER);
		CGlobal::GetInstance()->objects.push_back(Torch);
		
		break;
	}
	case SCENE_STATE_SECOND:
	{
		gameState = 1;
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		simon->SetPosition(100, 402-62);
		CGlobal::GetInstance()->objects.push_back(simon);
		CBrick *brick = new CBrick();
		CStair *stair = new CStair();
		for (int i = 0; i < 105; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(i * 32, 402);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		//first stair
		CHiddenObjects *hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setStairState(2);
		hidenObject->setBoundBox(200, 5);
		hidenObject->SetPosition(1250 - 32, 402 - 6);
		CGlobal::GetInstance()->objects.push_back(hidenObject);
		for (int i = 0; i < 4; i++)
		{

			stair = new CStair();
			stair->SetPosition(1250 + i * 32, 370 - i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setBoundBox(32, 32);
		hidenObject->setStairState(3);
		hidenObject->SetPosition(1250 + 32 * 4, 370 - 32 * 7 + 37);
		CGlobal::GetInstance()->objects.push_back(hidenObject);


		for (int i = 0; i < 3; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(1250 + 4 * 32 + i * 32, 274);
			if (i == 0) brick->panJump = true;
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		//stair 2
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setBoundBox(96, 5);
		hidenObject->setStairState(2);
		hidenObject->SetPosition(1442 - 32, 274 - 10);
		CGlobal::GetInstance()->objects.push_back(hidenObject);
		for (int i = 0; i < 2; i++)
		{

			stair = new CStair();
			stair->SetPosition(1442 + 32 * i, 242 - i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setStairState(3);
		hidenObject->setBoundBox(32, 32);
		hidenObject->SetPosition(1442 + 64, 212 - 64 * 2 + 35);
		CGlobal::GetInstance()->objects.push_back(hidenObject);




		for (int i = 0; i < 10; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			if (i == 0 || i == 9) brick->panJump = true;
			brick->SetPosition(1442 + 64 + i * 32, 210);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		CPanther* panther = new CPanther();
		panther->SetPosition(1442 + 64 + 5 * 32, 210 - 35);
		panther->SetState(PANTHER_STATE_LIEDOWN);
		panther->setNx(-1);
		CGlobal::GetInstance()->objects.push_back(panther);

		//stair 3

		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setStairState(1);
		hidenObject->setBoundBox(96, 5);
		hidenObject->SetPosition(1890 - 64, 210 + 32 * 2 - 6);
		CGlobal::GetInstance()->objects.push_back(hidenObject);

		for (int i = 0; i < 2; i++)
		{

			stair = new CStair();
			stair->setNx(-1);
			stair->SetPosition(1794 + 32 + i * 32, 210 + i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setStairState(4);
		hidenObject->setBoundBox(32, 32);
		hidenObject->SetPosition(1890 - 96, 212 - 64 * 2 + 35);
		CGlobal::GetInstance()->objects.push_back(hidenObject);


		for (int i = 0; i < 6; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			if (i == 0 || i == 5) brick->panJump = true;
			brick->SetPosition(1890 - 32 + i * 32, 210 + 32 * 2);
			CGlobal::GetInstance()->objects.push_back(brick);

		}



		//  stair 4
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setStairState(2);
		hidenObject->setBoundBox(96, 5);
		hidenObject->SetPosition(2590 - 32, 402 - 6);
		CGlobal::GetInstance()->objects.push_back(hidenObject);
		for (int i = 0; i < 6; i++)
		{

			stair = new CStair();
			stair->SetPosition(2590 + i * 32, 370 - i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setBoundBox(32, 32);
		hidenObject->setStairState(3);
		hidenObject->SetPosition(2590 + 32 * 6, 370 - 32 * 9 + 37);
		CGlobal::GetInstance()->objects.push_back(hidenObject);
		for (int i = 0; i < 17; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(2590 + 32 * 6 + i * 32, 370 - 5 * 32);
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		for (int i = 0; i < 20; i++)
		{
			if (i == 4) continue;
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(3104 - 64 + i * 32, 402);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				brick = new CBrick();
				brick->SetState(BRICK_STATE_MODERN);
				brick->SetPosition(3584 + j * 32, 402 - i * 32);
				CGlobal::GetInstance()->objects.push_back(brick);
			}

		}

		for (int i = 0; i < 4; i++)
		{

			stair = new CStair();
			stair->setNx(-1);
			stair->SetPosition(3424 + i * 32, 274 + i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		for (int i = 0; i < 3; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(3296 + 32 + i * 32, 274);
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		CDoor *door = new CDoor();
		door->SetState(DOOR_STAITE_SMAILDOOR);
		door->SetPosition(2590 + 32 * 6 + 9 * 32, 370 - 5 * 32 - DOOR_BBOX_HEIGHT);
		CGlobal::GetInstance()->objects.push_back(door);

		panther = new CPanther();
		panther->SetPosition(1250 + 4 * 32 + 38, 274 - 33);
		panther->SetState(PANTHER_STATE_LIEDOWN);
		panther->setNx(-1);
		CGlobal::GetInstance()->objects.push_back(panther);

		panther = new CPanther();
		panther->SetPosition(1890 + 3 * 32 , 210 + 32 * 2-50);
		panther->SetState(PANTHER_STATE_LIEDOWN);
		panther->setNx(-1);
		CGlobal::GetInstance()->objects.push_back(panther);

		//and Goombas 
		for (int i = 0; i < 3; i++)
		{
			CGhoul* ghoul = new CGhoul();
			ghoul->SetPosition(500 + i * 60, 370 - GHOUL_BBOX_HEIGHT);
			ghoul->SetState(GHOUL_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}

		for (int i = 0; i < 3; i++)
		{
			CGhoul* ghoul = new CGhoul();
			ghoul->SetPosition(1000 + i *60 , 370 - GHOUL_BBOX_HEIGHT);
			ghoul->SetState(GHOUL_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}

		for (int i = 0; i < 3; i++)
		{
			CGhoul* ghoul = new CGhoul();
			ghoul->SetPosition(1500 + i * 80, 370 - GHOUL_BBOX_HEIGHT);
			ghoul->SetState(GHOUL_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}


		for (int i = 0; i < 3; i++)
		{
			CGhoul* ghoul = new CGhoul();
			ghoul->SetPosition(2000 + i * 80, 370 - GHOUL_BBOX_HEIGHT);
			ghoul->SetState(GHOUL_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}

		for (int i = 0; i < 3; i++)
		{
			CGhoul* ghoul = new CGhoul();
			ghoul->SetPosition(2500 + i * 80, 370 - GHOUL_BBOX_HEIGHT);
			ghoul->SetState(GHOUL_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}
		break;
	}
	case SCENE_STATE_THIRD:
	{
		gameState = 2;
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		
		simon->SetPosition(3200, 146);
		if (backscene == true)
		{
			simon->SetPosition(3160, 368);
			backscene = false;
		}
		CCamera::GetInstance()->setCamera(3074, 0);
		CGlobal::GetInstance()->objects.push_back(simon);
		CBrick *brick ;
		CStair *stair ;

		CBat *bat;

		CHiddenObjects *hidenObject = new CHiddenObjects();
		for (int i = 0; i < 40; i++)
		{
			if (i == 4) continue;
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(3104 - 64 + i * 32, 402);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				brick = new CBrick();
				brick->SetState(BRICK_STATE_MODERN);
				brick->SetPosition(3584 + j * 32, 402 - i * 32);
				CGlobal::GetInstance()->objects.push_back(brick);
			}

		}



		for (int i = 0; i < 7; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(3104 + i * 32, 210);
			CGlobal::GetInstance()->objects.push_back(brick);
		}
		for (int i = 0; i < 3; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(3296 + 32 + i * 32, 274);
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		//stair 1

		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setStairState(1);
		hidenObject->setBoundBox(96, 5);
		hidenObject->SetPosition(3424 + 2 * 32, 274 + 4 * 32 - 10);
		CGlobal::GetInstance()->objects.push_back(hidenObject);

		for (int i = 0; i < 4; i++)
		{

			stair = new CStair();
			stair->setNx(-1);
			stair->SetPosition(3424 + i * 32, 274 + i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setStairState(4);
		hidenObject->setBoundBox(32, 32);
		hidenObject->SetPosition(3424 - 35, 274 - 61 - 30);
		CGlobal::GetInstance()->objects.push_back(hidenObject);


		//stair 2;

		stair = new CStair();
		stair->setNx(-1);
		stair->SetPosition(3104 - 64 + 4 * 32, 402);
		CGlobal::GetInstance()->objects.push_back(stair);

		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_TOP);
		hidenObject->setStairState(4);
		hidenObject->setBoundBox(32 + 5, 32);
		hidenObject->SetPosition(3104 - 64 + 3 * 32 - 5, 402 - 92);
		CGlobal::GetInstance()->objects.push_back(hidenObject);



		bat = new CBat();
		bat->SetState(BAT_STATE_IDLE);
		bat->setNx(-1);
		bat->SetPosition(3074 + 640, 120);
		bat->setBatoy(120);
		CGlobal::GetInstance()->objects.push_back(bat);



		bat = new CBat();
		bat->SetState(BAT_STATE_IDLE);
		bat->setNx(-1);
		bat->SetPosition(3074 + 640 + 100, 300);
		bat->setBatoy(300);
		CGlobal::GetInstance()->objects.push_back(bat);

		break;
	}
	case SCENE_STATE_FOURTH:
	{
		gameState = 2;
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		simon->SetPosition(114, 70);
		CCamera::GetInstance()->setCamera(0, 0);
		CGlobal::GetInstance()->objects.push_back(simon);
		CBrick *brick;
		CStair *stair;
		CFishmen *fishman;
		CHiddenObjects *hidenObject;
		for (int i = 0; i < 30; i++)
		{
			if (i==14 || i == 15 || i == 18 || i == 19 || i == 28 || i == 29)
			{
				continue;
			}
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(i * 32, 160+82);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		for (int i = 0; i < 2; i++)
		{
			brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(6* 32+i*32, 160 + 82-64);
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		for (int i = 0; i < 3; i++)
		{

			stair = new CStair();
			stair->setNx(-1);
			stair->SetPosition(97 + i * 32, i * 32+82);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		hidenObject = new CHiddenObjects();
		hidenObject->SetState(HO_STATE_STAIR_BOTTOM);
		hidenObject->setStairState(1);
		hidenObject->setBoundBox(96, 5);
		hidenObject->SetPosition(97+32, 3*32+82-8);
		CGlobal::GetInstance()->objects.push_back(hidenObject);


		fishman = new CFishmen();
		fishman->SetPosition(800, 300);
		fishman->SetState(FISHMAN_STATE_JUMP);
		fishman->setNx(-1);
		CGlobal::GetInstance()->objects.push_back(fishman);
		break;
	}
	}


}

void CSceneManager::sceneUpdate()
{
	if (ReplaceScene==true)
	{
		CCamera::GetInstance()->ResetCam();
	
		if (backscene == true)
		{
			currentScene -= 100;
		}
		else
		{
			currentScene += 100;
		}
		if (currentScene>=SCENE_STATE_FOURTH)
		{
			currentScene = SCENE_STATE_FOURTH;
		}
		
		initScene();
		ReplaceScene = false;
	}
}

void CSceneManager::Render()
{

	RenderMap();
	float camX, camY;
	CCamera::GetInstance()->getCamera(camX, camY);
	for (int i = 1; i < CGlobal::GetInstance()->objects.size(); i++)
	{
		if ((CGlobal::GetInstance()->objects[i]->x>(int)camX - 32 && CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH)&& CGlobal::GetInstance()->objects[i]->y>80)
		{
			CGlobal::GetInstance()->objects[i]->Render();
		}

	}
		
	CGlobal::GetInstance()->objects[0]->Render();
	if (currentScene== SCENE_STATE_FIRST)
	{
		CGlobal::GetInstance()->objects[1]->Render();
	}
	CGameUI::GetInstance()->Render();

}
void CSceneManager::Update(DWORD dt)
{


	CSIMON *simon = CSIMON::GetInstance();
	CGameUI::GetInstance()->Update();

	// We know that SIMON is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (currentScene== SCENE_STATE_FIRST)
	{
		if (simon->x >= 640 / 2 - 60 && simon->x < 1530 - 640 / 2 - 60)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
		}
	}
	else if (currentScene==SCENE_STATE_SECOND)
	{
		if (simon->x >= 640 / 2 - 60 && simon->x < 3070 - 640 / 2)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
		}
	}
	else if (currentScene == SCENE_STATE_THIRD)
	{
		if (simon->x >= 3074 + 640 / 2 - 60 && simon->x < 4095 - 640 / 2)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
		}
	
	}
	else if (currentScene==SCENE_STATE_FOURTH)
	{
		if (simon->x >= 640 / 2 - 60 && simon->x < 1056 - 640 / 2 - 64*2)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
		}
	}
	vector<LPGAMEOBJECT> coObjects;


	if (simon->x>1320 && currentScene == SCENE_STATE_FIRST)
	{
		if (simon->x<1450)
		{
			simon->autoWalk = true;
		}
		else
		{
			simon->autoWalk = false;
			DebugOut(L"*********Attention scene is replaces*******\n");
			ReplaceScene = true;
			sceneUpdate();
		}
	
	}
	if (simon->x>3070-64 && currentScene == SCENE_STATE_SECOND)
	{
		simon->removeControl = true;
		if (CCamera::GetInstance()->autoCamera(2900)==true)
		{
			simon->removeControl = false;
			simon->autoWalk = true;
		}
		if (simon->x > 3200)
		{
			simon->removeControl = true;
			simon->autoWalk = false;
			simon->SetState(SIMON_STATE_IDLE);
			if (CCamera::GetInstance()->autoCamera(3074) == true)
			{
				simon->removeControl = false;
				DebugOut(L"*********Attention scene is replaces*******\n");
				ReplaceScene = true;
				sceneUpdate();
			}
		}
	}
	if ((int)simon->y> 370 && currentScene == SCENE_STATE_THIRD)
	{
		DebugOut(L"*********Attention scene is replaces*******\n");
		ReplaceScene = true;
		sceneUpdate();
	}
	
	if ((int)simon->y<65  && currentScene == SCENE_STATE_FOURTH)
	{
		DebugOut(L"*********Attention scene is replaces*******\n");
		ReplaceScene = true;
		backscene = true;
		sceneUpdate();
	}

	//dọn rác trc khi update
	for (int i = 0; i < CGlobal::GetInstance()->objects.size(); i++)
	{

		if (CGlobal::GetInstance()->objects[i]->isRemove == true)
		{
			CGlobal::GetInstance()->objects.erase(CGlobal::GetInstance()->objects.begin() + i);
			DebugOut(L"==========Object bi xoa =================\n");
		}
	}
	float camX, camY;
	CCamera::GetInstance()->getCamera(camX, camY);

	//Lấy list object để xét va chạm
	//Chỉ lấy những object nằm trong ViewPort
	for (int  i= 1; i < CGlobal::GetInstance()->objects.size(); i++)
	{
		if (dynamic_cast<CStair *>(CGlobal::GetInstance()->objects[i]))
		{
			continue;
		}

		if (dynamic_cast<CGround *>(CGlobal::GetInstance()->objects[i]))
		{
			coObjects.push_back(CGlobal::GetInstance()->objects[i]);
			continue;
		}
		
		if (CGlobal::GetInstance()->objects[i]->x>(int)camX-64 &&CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH+64)
		{
			coObjects.push_back(CGlobal::GetInstance()->objects[i]);
		}
	}


	//DebugOut(L"CoObsize=%d \n Obsize=%d \n", coObjects.size(), CGlobal::GetInstance()->objects.size());
	for (int i = 0; i < CGlobal::GetInstance()->objects.size(); i++)
	{
		//-64,+64 có 1 số object nếu vượt khỏi vp sẽ xóa khỏi list,
		// nếu k + và - thêm sẽ ngừng update nên k xét đc điều kiện
		// bên trong các object
		if (dynamic_cast<CGround *>(CGlobal::GetInstance()->objects[i]))
		{
			CGlobal::GetInstance()->objects[i]->Update(dt, &coObjects);
			continue;
		}
		if (CGlobal::GetInstance()->objects[i]->x>(int)camX - 64 && CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH + 64)
		{
			CGlobal::GetInstance()->objects[i]->Update(dt, &coObjects);
		}

	}

}

CSceneManager::CSceneManager()
{
	
	ReplaceScene = false;
	switch (1)
	{
	case 1:
	{
		currentScene = SCENE_STATE_FIRST;
		break;
	}
	case 2:
	{
		currentScene = SCENE_STATE_SECOND;
		break;
	}
	case 3:
	{
		currentScene = SCENE_STATE_THIRD;
		break;
	}
	case 4:
	{
		currentScene = SCENE_STATE_FOURTH;
		break;
	}
	}
}


CSceneManager::~CSceneManager()
{
}
