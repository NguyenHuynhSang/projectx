﻿/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 04 - COLLISION

This sample illustrates how to:

1/ Implement SweptAABB algorithm between moving objects
2/ Implement a simple (yet effective) collision frame work

Key functions:
CGame::SweptAABB
CGameObject::SweptAABBEx
CGameObject::CalcPotentialCollisions
CGameObject::FilterCollision

CGameObject::GetBoundingBox

================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Global.h"
#include "Simon.h"
#include "ghoul.h"
#include "Torch.h"
#include "TileMap.h"
#include"Camera.h"
#include"SceneManager.h"
#define WINDOW_CLASS_NAME L"CastleVania"
#define MAIN_WINDOW_TITLE L"CastleVania"
///Clear background to black
#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

// kích thước mặc định của Window


#define MAX_FRAME_RATE 120



CGame *game;
CSIMON *SIMON;
CSceneManager *scene;
class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (SIMON->removeControl==true) return;

	switch (KeyCode)
	{
	case DIK_C:
		if (SIMON->GetState() == SIMON_STATE_UPWHIP ||SIMON->getHeart()==0 || SIMON->getcurrentSubWeapon()==0|| SIMON->onStair==true)
		{
			//DebugOut(L"\n");
			return;
		}
		SIMON->isUsesW = true;
		SIMON->isDelay = true;
		SIMON->SetState(SIMON_STATE_STAND_FIGHTING);
		break;


	case DIK_F:
		//DebugOut(L"Press Fighting  \n");
		if (SIMON->GetState()==SIMON_STATE_UPWHIP)
		{
			return;
		}
		if (SIMON->GetState() == SIMON_STATE_IDLE_UP_STAIR && SIMON->vx == 0 && SIMON->vy == 0)
		{
			SIMON->SetState(SIMON_STATE_UPSTAIR_FIGHTING);
			return;
		}
		if (SIMON->onStair==true)
		{
			return;
		}
		SIMON->SetState(SIMON_STATE_STAND_FIGHTING);
		break;
	case DIK_SPACE:
		if (SIMON->GetState() == SIMON_STATE_JUMP || SIMON->GetState()== SIMON_STATE_STAND_FIGHTING || SIMON->GetState() == SIMON_STATE_UPWHIP ||SIMON->isSitting==true|| SIMON->autoWalk==true)
		{
			return;
		}
			SIMON->SetState(SIMON_STATE_JUMP);
		
		break;
	case DIK_A: // reset
		SIMON->SetState(SIMON_STATE_IDLE);
		SIMON->onStair = false;
		SIMON->SetPosition(50.0f, 50.0f);
		///fix cam when reset
		game->setCam(0,NULL);
		SIMON->SetSpeed(0, 0);
		break;
	}

		
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{

}
bool down = false;
bool pressUD=false;
int checkStair = 0;
///bắt sự kiện bàn phím và cập nhật trạng thái
void CSampleKeyHander::KeyState(BYTE *states)
{
	// Nên dùng time để xử lý vì dùng frame cuối vừa đến frame cuối ani sẽ dừng
	//mất delay time cần có phương pháp hiệu quả hon
	/// xong

	SIMON->isJump=false;
	// disable control key when SIMON die 
	if (SIMON->GetState() == SIMON_STATE_DIE) return;
	if (SIMON->GetState() == SIMON_STATE_HIT_ENERMY) return;
	if (SIMON->removeControl == true) return;

	if (SIMON->autoWalk==true)
	{
		return;
	}
	if (SIMON->isUpWhip == true)
	{
		SIMON->SetState(SIMON_STATE_UPWHIP);
		DebugOut(L"*****State Upwhip***** \n");
		return;
	}
	if (SIMON->GetState() == SIMON_STATE_UP_STAIR)
	{
		if (CAnimations::GetInstance()->Get(SIMON_IDANI_WUPSTAIR)->getCurrentFrame() != 2)
		{
			return;
		}
		CAnimations::GetInstance()->Get(SIMON_IDANI_WUPSTAIR)->ResetCurrentFrame();
	}
	if (SIMON->GetState() == SIMON_STATE_DOWN_STAIR)
	{
		//DebugOut(L"Delay down stair \n");
		if (CAnimations::GetInstance()->Get(SIMON_IDANI_WDOWNSTAIR)->getCurrentFrame() != 2)
		{
			return;
		}
		CAnimations::GetInstance()->Get(SIMON_IDANI_WDOWNSTAIR)->ResetCurrentFrame();
	}
	//DebugOut(L"SIMON_X=%d    stairX=%d \n", (int)SIMON->x, SIMON->stair_X);
	//kiểm tra để auto stair tại đây để remove control
	if (pressUD==true)
	{
		if (SIMON->bottomStair ==true && checkStair==1)
		{
			if (SIMON->onStair!=true)
			{
				if ((int)SIMON->x>SIMON->stair_X + 2)
				{
					SIMON->SetState(SIMON_STATE_WALKING_LEFT);
					return;
				}
				else
				{
					if ((int)SIMON->x<SIMON->stair_X)
					{
						SIMON->SetState(SIMON_STATE_WALKING_RIGHT);
						return;
					}
					SIMON->setNX(1);
					SIMON->onStair = true;
					SIMON->SetState(SIMON_STATE_UP_STAIR);
					CAnimations::GetInstance()->Get(SIMON_IDANI_WUPSTAIR)->ResetCurrentFrame();
					pressUD = false;
					return;
				}
			}
			
		}
		else if (SIMON->topStair==true )
		{
			if (SIMON->onStair != true)
			{
				if ((int)SIMON->x < SIMON->stair_X-2 )
				{

					if (SIMON->stairState==4 || SIMON->stairState == 3)
					{
						SIMON->SetState(SIMON_STATE_WALKING_RIGHT);
						return;
					}
				}
				else
				{
					if ((int)SIMON->x > SIMON->stair_X )
					{

						if (SIMON->stairState == 3)
						{
							SIMON->SetState(SIMON_STATE_WALKING_LEFT);
							return;
						}
					}
					SIMON->setNX(1);
					SIMON->onStair = true;
					SIMON->SetState(SIMON_STATE_DOWN_STAIR);
					CAnimations::GetInstance()->Get(SIMON_IDANI_WDOWNSTAIR)->ResetCurrentFrame();
					pressUD = false;
					return;
				}
			}
		}
		
	}
	pressUD = false;
	checkStair = 0;
	if (SIMON->GetState()==SIMON_STATE_UPSTAIR_FIGHTING)
	{
		if (CAnimations::GetInstance()->Get(SIMON_IDANI_UPSTAIR_FIGHT)->getCurrentFrame() != 3)
		{
			//	DebugOut(L"State fighting \n");
			return;
		}
		else
		{
			SIMON->isDelay = false;
			// sửa lỗi bị delay
			CAnimations::GetInstance()->Get(SIMON_IDANI_UPSTAIR_FIGHT)->setCurrentFrame(-1);
		}
	}
	if (SIMON->GetState() == SIMON_STATE_STAND_FIGHTING)
	{
		if (SIMON->isSitting==true)
		{
			if (CAnimations::GetInstance()->Get(506)->getCurrentFrame() != 3)
			{
				return;
			}
			else
			{
				SIMON->isDelay = false;
				CAnimations::GetInstance()->Get(506)->setCurrentFrame(-1);
			}
		}
		else
		{
			if (CAnimations::GetInstance()->Get(502)->getCurrentFrame() != 3)
			{
			//	DebugOut(L"State fighting \n");
				return;
			}
			else
			{
				SIMON->isDelay = false;
				// sửa lỗi bị delay
				CAnimations::GetInstance()->Get(502)->setCurrentFrame(-1);
			}
		}

		
	}
	if (SIMON->GetState()==SIMON_STATE_JUMP && SIMON->isTouchGr()==false)
	{
	
			SIMON->isJump = true;
			return;
	}

	// xử lý trên cầu thang
	if (game->IsKeyDown(DIK_UP))
	{
		if (SIMON->bottomStair == true)
		{
			checkStair = 1;
			down = false;
			pressUD = true;
			if (SIMON->onStair ==false)
			{
				return;
			}
			SIMON->onStair = true;
		}
		if (SIMON->onStair==true)
		{
			
			down = false;
			SIMON->SetState(SIMON_STATE_UP_STAIR);
			SIMON->isUpStair = true;
			return;

		}
	
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (SIMON->topStair == true)
		{
			pressUD = true;
			down = true;
			if (SIMON->onStair == false)
			{
				return;
			}
			SIMON->onStair = true;
		}
		if (SIMON->onStair==true)
		{
			down = true;
			//DebugOut(L"Simon down stair\n");
			SIMON->SetState(SIMON_STATE_DOWN_STAIR);
			return;
		}
		
	}
	else
	{
		if (SIMON->onStair==true && down!=true)
		{
			//DebugOut(L"idle up stair \n");
			SIMON->SetState(SIMON_STATE_IDLE_UP_STAIR);
			return;
		}
		if (SIMON->onStair == true && down==true)
		{
			//DebugOut(L"idle down stair \n");
			SIMON->SetState(SIMON_STATE_IDLE_DOWN_STAIR);
			return;
		}
		
	}



	// Xử lý di chuyển
	if (game->IsKeyDown(DIK_DOWN))
	{
		SIMON->isJump = true;
		// set state tranh truong hop nguoi dung bam cung luc ngoi va toi
		SIMON->SetState(SIMON_STATE_IDLE);
		SIMON->isSitting = true;
		return;
	}
	else { SIMON->isSitting = false; }



	if (game->IsKeyDown(DIK_RIGHT))         //arrow right
	{
		SIMON->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))       //arrow left
	{
		SIMON->SetState(SIMON_STATE_WALKING_LEFT);
	}

	else {
		SIMON->SetState(SIMON_STATE_IDLE);
	}
	

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
Load all game resources
In this example: load textures, sprites, animations and SIMON object

TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/

/*====================================================================
// LOGIC CỦA ANIMATION
// Ví dụ SIMON
// ANIMATION(đi,chạy,nhảy....) sẽ được thêm vào ngăn xếp
// Các animation của simon đã được định nghĩa sẵn trong class là
// số tt của ani đã được thêm vào ngăn xếp
// Lúc này muốn animation hoạt động đúng xem phương thức Render của
// class simon
//===================================================================*/



/*
Update world status for this frame
dt: time period between beginning of last frame and beginning of this frame
*/

/*
Render a frame
*/

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		scene->GetInstance()->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}


HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			scene->GetInstance()->Update(dt);
			Render();
			
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);
		
	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	scene = CSceneManager::GetInstance();
	SIMON = CSIMON::GetInstance();
	scene->GetInstance()->LoadResource();
	scene->initScene();
	


	/// hien ra giua man hinh may tinh 
	// chua sua duoc


	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT , SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}