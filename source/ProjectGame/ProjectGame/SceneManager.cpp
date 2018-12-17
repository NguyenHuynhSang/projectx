﻿#include "SceneManager.h"
#include"Camera.h"
#include"Stair.h"
#include"HiddenObjects.h"
#include"Effect.h"
#include"Game.h"
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
	textures->Add(ID_BACKGROUND_LV1, L"data\\map\\tileset.BMP", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_BACKGROUND_LV2, L"data\\map\\lv2.BMP", D3DCOLOR_XRGB(255, 255, 255));
	CItem *items = new CItem();
	items->LoadResource();

	CDagger *dagger = new CDagger();
	dagger->LoadResource();

	CBrick *brick = new CBrick();
	brick->LoadResource();

	CGhoul *ghoul = new CGhoul();
	ghoul->LoadResource();

	CTorch *torch = new CTorch();
	torch->LoadResource();

	CStair *stair = new CStair();
	stair->LoadResource();

	CEffect *eff = new CEffect();
	eff->LoadResource();
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
		tileG->SetMSize(1536, 384);
		tileG->SetTileSetHeight(640, 128);
		tileG->LoadTile(MAP_MATRIXPATH_SCENE1, texMap);

		break;
	}
	case SCENE_STATE_SECOND:
	{
	
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_BACKGROUND_LV2); //tex Map
		tileG = new CTileMap();
		tileG->SetMSize(5632, 768);
		tileG->SetTileSetHeight(640, 192);
		tileG->LoadTile(MAP_MATRIXPATH_SCENE2, texMap);
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
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		simon->SetPosition(100, 250+25);
		CGlobal::GetInstance()->objects.push_back(simon);
		for (int i = 0; i < 1536 / 32 + 4; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetPosition(0 + i*32.0f, 350+25);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		CTorch* Torch = new CTorch();
		Torch->SetPosition(0 + 450, 350 - 60+25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_BHEART);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();
		Torch->SetPosition(750, 350 - 60+25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_NWHIP);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();
		Torch->SetPosition(950, 350 - 60+25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_NWHIP);
		CGlobal::GetInstance()->objects.push_back(Torch);

		Torch = new CTorch();;
		Torch->SetPosition(1200, 350 - 60+25);
		Torch->SetState(TORCH_STATE_BURNING);
		Torch->setItemState(ITEM_STATE_DANGER);
		CGlobal::GetInstance()->objects.push_back(Torch);

		break;
	}
	case SCENE_STATE_SECOND:
	{
		LoadMap();
		CGlobal::GetInstance()->objects.clear();
		CSIMON *simon = CSIMON::GetInstance();
		simon->SetPosition(842 + 6 * 32, 370-32);
		CGlobal::GetInstance()->objects.push_back(simon);
		for (int i = 0; i <100; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(0 + i*30.0f, 402);
			CGlobal::GetInstance()->objects.push_back(brick);
		}

		//first stair
		CHiddenObjects *hobj = new CHiddenObjects();
		hobj->SetState(HO_STATE_STAIR_BOTTOM);
		hobj->setStairState(2);
		hobj->setBoundBox(96, 5);
		hobj->SetPosition(1250-32,402-6);
		CGlobal::GetInstance()->objects.push_back(hobj);
		for (int i = 0; i < 4; i++)
		{
			
			CStair *stair = new CStair();
			stair->SetPosition(1250+i*32, 370-i*32 );
			if (i==3)
			{
				stair->beginStair = true;
			}
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		CHiddenObjects *hobj2 = new CHiddenObjects();
		hobj2->SetState(HO_STATE_STAIR_TOP);
		hobj2->setBoundBox(32, 32);
		hobj2->setStairState(3);
		hobj2->SetPosition(1250 +32*4, 370 - 32*7 + 37);
		CGlobal::GetInstance()->objects.push_back(hobj2);


		for (int i = 0; i <3; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(1250+4*32+i*32, 274);
			CGlobal::GetInstance()->objects.push_back(brick);
		}


		//stair 2
		CHiddenObjects *hobj3 = new CHiddenObjects();
		hobj3->SetState(HO_STATE_STAIR_BOTTOM);
		hobj3->setBoundBox(96, 5);
		hobj3->setStairState(2);
		hobj3->SetPosition(1442-32, 274 -5);
		CGlobal::GetInstance()->objects.push_back(hobj3);
		for (int i = 0; i < 2; i++)
		{
			
			CStair *stair = new CStair();
			if (i == 1)
			{
				stair->beginStair = true;
			}
			stair->SetPosition(1442+32*i, 242 - i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		CHiddenObjects *hobj4 = new CHiddenObjects();
		hobj4->SetState(HO_STATE_STAIR_TOP);
		hobj4->setStairState(3);
		hobj4->setBoundBox(32, 32);
		hobj4->SetPosition(1442+64, 212-64*2 + 35);
		CGlobal::GetInstance()->objects.push_back(hobj4);


	

		for (int i = 0; i <10; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(1442 +  64 + i * 32, 210);
			CGlobal::GetInstance()->objects.push_back(brick);
		}
	
		//stair 3

		CHiddenObjects *hobj5 = new CHiddenObjects();
		hobj5->SetState(HO_STATE_STAIR_BOTTOM);
		hobj5->setStairState(1);
		hobj5->setBoundBox(96, 5);
		hobj5->SetPosition(1890 -64, 210 + 32 * 2-6);
		CGlobal::GetInstance()->objects.push_back(hobj5);

		for (int i = 0; i < 2; i++)
		{

			CStair *stair = new CStair();
			stair->setNx(-1);
			if (i == 1)
			{
				stair->beginStair = true;
			}
			stair->SetPosition(1794+32+ i * 32, 210+ i * 32);
			CGlobal::GetInstance()->objects.push_back(stair);
		}
		CHiddenObjects *hobj6 = new CHiddenObjects();
		hobj6->SetState(HO_STATE_STAIR_TOP);
		hobj6->setStairState(4);
		hobj6->setBoundBox(32, 32);
		hobj6->SetPosition(1890 -96, 212 - 64 * 2 + 35);
		CGlobal::GetInstance()->objects.push_back(hobj6);


		for (int i = 0; i <6; i++)
		{
			CBrick *brick = new CBrick();
			brick->SetState(BRICK_STATE_MODERN);
			brick->SetPosition(1890-32 + i * 32, 210+32*2);
			CGlobal::GetInstance()->objects.push_back(brick);
		}




		//and Goombas 
		for (int i = 0; i < 7; i++)
		{
			CGhoul* ghoul = new CGhoul();

			ghoul->SetPosition(125 + i * 500, 370-GOOMBA_BBOX_HEIGHT);
			ghoul->SetState(GOOMBA_STATE_WALKING);
			CGlobal::GetInstance()->objects.push_back(ghoul);
		}



		break;
	}
	case SCENE_STATE_THIRD:
	{
		break;
	}

	}



}

void CSceneManager::sceneUpdate()
{
	if (ReplaceScene==true)
	{
		CCamera::GetInstance()->ResetCam();
		currentScene += 100;
		if (currentScene>=SCENE_STATE_THIRD)
		{
			currentScene = SCENE_STATE_THIRD;
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
		if (CGlobal::GetInstance()->objects[i]->x>(int)camX - 32 && CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH)
		{
			CGlobal::GetInstance()->objects[i]->Render();
		}

	}

	//render Simon
	CGlobal::GetInstance()->objects[0]->Render();

}

void CSceneManager::Update(DWORD dt)
{
	CSIMON *simon = CSIMON::GetInstance();
	// We know that SIMON is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (currentScene== SCENE_STATE_FIRST)
	{
		if (simon->x >= 640 / 2 - 60 && simon->x < 1530 - 640 / 2 - 60)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
			CCamera::GetInstance()->isCamMove = true;
		}
		else
		{
			CCamera::GetInstance()->isCamMove = false;
		}
	}
	else
	{
		if (simon->x >= 640 / 2 - 60 && simon->x < 5500 - 640 / 2 - 60)
		{
			CCamera::GetInstance()->setCamera(simon->x - SCREEN_WIDTH / 2 + 62, 0);
			CCamera::GetInstance()->isCamMove = true;
		}
		else
		{
			CCamera::GetInstance()->isCamMove = false;
		}
	}
	vector<LPGAMEOBJECT> coObjects, sceneObject;


	if (simon->x>1406 && currentScene == SCENE_STATE_FIRST)
	{
		sceneObject.clear();
		DebugOut(L"*********Attention scene is replaces*******\n");
		ReplaceScene = true;
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
	for (int i = 1; i < CGlobal::GetInstance()->objects.size(); i++)
	{
		if (dynamic_cast<CStair *>(CGlobal::GetInstance()->objects[i]))
		{
			continue;
		}

		if (CGlobal::GetInstance()->objects[i]->x>(int)camX &&CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH)
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
		if (CGlobal::GetInstance()->objects[i]->x>(int)camX - 64 && CGlobal::GetInstance()->objects[i]->x<(int)camX + SCREEN_WIDTH + 64)
		{
			CGlobal::GetInstance()->objects[i]->Update(dt, &coObjects);
		}

	}

}

CSceneManager::CSceneManager()
{
	ReplaceScene = false;
	currentScene = SCENE_STATE_FIRST;
	//currentScene = SCENE_STATE_SECOND;
}


CSceneManager::~CSceneManager()
{
}
