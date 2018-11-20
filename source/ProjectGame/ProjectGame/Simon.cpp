﻿#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "Zombie.h"
#include"Whip.h"
#include"Torch.h"
#include"Brick.h"
void CSIMON::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	coEventsResult.clear();
	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}
	if (state==SIMON_STAGE_STAND_FIGHTING)
	{
		//DebugOut(L"Simon Pos x= %d y=%d   \n",x,y);
		// chi update khi dang o frame cuoi ==> luc roi danh ra
		if (CAnimations::GetInstance()->Get(ID_WHIP_ANI)->getCurrentFrame() == 2)
		{
			whip->Update(dt, coObjects);
		}
		
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//xu ly va cham voi nen nha

			if (dynamic_cast<CBrick *>(e->obj))
			{
				if (e->ny != 0)
				{
					//DebugOut(L"Va cham \n");
					x += min_tx*dx + nx*0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
					y += min_ty*dy + ny*0.4f;
					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;
				}

			}





			//va cham voi candle
			//khi Simon đi qua CTorch, set chỉ số alpha lại thành 128
			if (dynamic_cast<CTorch *>(e->obj))
			{
			//	DebugOut(L"Tourch \n");
				if (e->nx!=0)
				{
					x += dx;//Vẫn cho simon đi tới khi va chạm theo phương ngang
					untouchable == true; // cho simon trở nên mờ ảo 
				}
				if (e->ny!=0)// xét nếu va chạm theo phương thẳng đứng
				{
					y += dy;// vẫn cho simon rơi xuống bị delay khi rơi xuống tại đây??
				}
				
			}



			//Va cham voi ghost
			if (dynamic_cast<CGoomba *>(e->obj))
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (e->nx!=0)
				{
					//
					//DebugOut(L"Cos event running");
					/*vx = 0.2 * 2;*/
					x += dx;
				}
				// jump on top >> kill Goomba and deflect a bit 

				if (e->ny < 0)
				{

					y += dy;

				/*	if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -SIMON_JUMP_DEFLECT_SPEED*2;
					}*/
				}
				else if (e->nx != 0)
				{
					/*if (untouchable==0)
					{
					if (goomba->GetState()!=GOOMBA_STATE_DIE)
					{
					if (level > SIMON_LEVEL_SMALL)
					{
					level = SIMON_LEVEL_SMALL;
					StartUntouchable();
					}
					else
					SetState(SIMON_STATE_DIE);
					}
					}*/
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CSIMON::Render()
{
	
	int ani;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	{
		//check Simon sits
		if (getStateforAniSitandJump == true)
		{
				ani = SIMON_ANI_SIT_JUMP;
				
		}
		else
		{
			if (vx!=0)
			{
				if (state == SIMON_STAGE_STAND_FIGHTING)
				{
					//DebugOut(L"simon fi");
					ani = SIMON_ANI_STAND_FIRE;
				}
				else
				{
					ani = SIMON_ANI_BIG_WALKING;
				}
			}
			else
			{
				if (state == SIMON_STAGE_STAND_FIGHTING)
				{
					//DebugOut(L"simon fi");
					ani = SIMON_ANI_STAND_FIRE;
				}
				else
				{
					ani = SIMON_ANI_BIG_IDLE;
				}
			}

		}
	}

	// Xu ly whip
	if (state == SIMON_STAGE_STAND_FIGHTING)
	{
		whip->setnx(nx);
		whip->SetPosition(x - 88, y);
		whip->Render();
	}
	else
	{
		//reset current frame
		CAnimations::GetInstance()->Get(555)->setCurrentFrame(-1);
	}


	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[ani]->Render(nx,x, y,alpha);
	// show boundingbox de check va cham
	RenderBoundingBox();
	
}


void CSIMON::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:

			DebugOut(L"[Line]:%d SIMON state jump",__LINE__);
			vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STAGE_STAND_FIGHTING:
		if (getStateforAniSitandJump==false)
		{
			vx = 0;	// dung khi simon dung vampie killer
		}
		
		
	
		break;
	}
}

//get bounding box dùng để xét va chạm vì khi vẽ sprite lên khung hình thường
//to hơn hình ảnh bên trong,
void CSIMON::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//x y của SIMON sẽ xét lại
	left = x-12;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH+10;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;
}

