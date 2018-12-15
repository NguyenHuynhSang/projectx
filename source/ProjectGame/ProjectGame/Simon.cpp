﻿#include <algorithm>
#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "ghoul.h"
#include"Whip.h"
#include"Torch.h"
#include"Brick.h"
#include"Item.h"
#include"Stair.h"
#include"HiddenObjects.h"
CSIMON *CSIMON::__instance = NULL;
CSIMON * CSIMON::GetInstance()
{
	if (__instance == NULL)	__instance = new CSIMON();
	return __instance;
}
void CSIMON::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (state!=SIMON_STATE_UP_STAIR && state != SIMON_STATE_DOWN_STAIR)
	{
		vy += SIMON_GRAVITY*dt;
		//isUpStair = false;
		//isDownStair = false;
	}
	if (state==SIMON_STATE_IDLE_UP_STAIR || state==SIMON_STATE_IDLE_DOWN_STAIR)
	{
		vy = 0;
		vx = 0;
		return;
	}
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	coEventsResult.clear();
	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (state==SIMON_STATE_STAND_FIGHTING)
	{
		
		//DebugOut(L"[line][%d]   :State fighting \n",__LINE__);
		if (isDelay==false)
		{
			//DebugOut(L"Simon Pos x= %d y=%d   \n",x,y);
			// chi update khi dang o frame cuoi ==> luc roi danh ra
			if (whip->GetState() == WHIP_STATE_WHITE)
			{
				if (CAnimations::GetInstance()->Get(WHIP_NOLMAL_ANI_ID)->getCurrentFrame() == 2)
				{
					whip->Update(dt, coObjects);
				}
			}
			else if (whip->GetState() == WHIP_STATE_BLUE)
			{
				if (CAnimations::GetInstance()->Get(WHIP_BLUE_ANI_ID)->getCurrentFrame() == 2)
				{
					whip->Update(dt, coObjects);
				}
			}
			else if (whip->GetState() == WHIP_STATE_RED)
			{
				if (CAnimations::GetInstance()->Get(WHIP_RED_ANI_ID)->getCurrentFrame() == 8)
				{
					whip->Update(dt, coObjects);
				}
			}
		}
		else
		{
			if (isSitting==true)
			{
				if (CAnimations::GetInstance()->Get(506)->getCurrentFrame() == 0)
				{
					if (isUsesW == true)
					{
						nHeart--;
						dagger = new CDagger();
						dagger->setNx(nx);
						dagger->SetPosition(x+10, y+12);
						CGlobal::GetInstance()->objects.push_back(dagger);
					}

					isUsesW = false;
				}
			}
			else
			{

				if (CAnimations::GetInstance()->Get(502)->getCurrentFrame() == 0)
				{
					if (isUsesW == true)
					{
						nHeart--;
						dagger = new CDagger();
						DebugOut(L"Create Dagger \n");
						dagger->setNx(nx);
						dagger->SetPosition(x, y);
						CGlobal::GetInstance()->objects.push_back(dagger);
					}

					isUsesW = false;
				}
			}
			

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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//xu ly va cham voi nen nha
			if (dynamic_cast<CBrick *>(e->obj))
			{
				if (e->ny < 0)
				{
					if (onStair == true)
					{
						state = SIMON_STATE_IDLE;
						onStair = false;
					}
					
					x += min_tx*dx + nx*0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
					y += min_ty*dy + ny*0.4f;
					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;
					if (state == SIMON_STATE_STAND_FIGHTING)
					{
						vx = 0;
					}
				}
				else if (e->ny>0)
				{
					y += dy;
					if (nx != 0) vx = 0;
				}
				if (e->nx != 0)
				{
					if (onStair == true)
					{
						x += dx;
						y += dy;
					}
				}
			
			}
			if (dynamic_cast<CStair *>(e->obj))
			{
				CStair *stair = dynamic_cast<CStair *>(e->obj);

				if (e->nx != 0)
				{
					x += dx;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
				}
				else if (e->ny != 0)
				{
					// nx*0.4f : need to push out a bit to avoid overlapping next frame
					//y += dy;
					if (stair->beginStair!=true)
					{
						y += dy;

					}
					x += dx;
				}
			}
			
			if (dynamic_cast<CHiddenObjects *>(e->obj))
			{
				DebugOut(L"Va cham \n");
				CHiddenObjects * f = dynamic_cast<CHiddenObjects*> (e->obj);
				if (e->nx!=0)
				{
					x += dx;
				}
				if (e->ny!=0)
				{
					if (f->GetState()==HO_STATE_STAIR_TOP)
					{
						if (onStair==true)
						{
							isUpStair = false;
							state = SIMON_STATE_IDLE;
							onStair = false;
						}
						
					}
					if (f->GetState() == HO_STATE_STAIR_BOTTOM)
					{
						

					}
					y += dy;
				}
			}
	
	
			// xử lý va chạm giữa Simon và item
			// trường hợp nếu item xuất hiện đúng vị trí
			//bbox của simon thuật toán sẽ chạy sai
			// cần có pp cho trường hợp đó ex:AABB
			if (dynamic_cast<CItem *>(e->obj))
			{
				DebugOut(L"\n ITEM HERE");
				CItem * f = dynamic_cast<CItem*> (e->obj);
				if (e->nx!=0)
				{
					DebugOut(L"Col with item \n");
					x += dx;
					if (f->GetState()==ITEM_STATE_BHEART)
					{
						nHeart += 5;
					}
					else
					if (f->GetState()==ITEM_STATE_NWHIP)
					{
						isUpWhip = true;
						DebugOut(L"Upwhiphere \n");
					}
					f->isRemove = true;
				}
				else if (e->ny!=0)
				{
					DebugOut(L"Col with item \n");
					y += dy;
					if (f->GetState() == ITEM_STATE_BHEART)
					{
						nHeart += 5;
					}
					else
					if (f->GetState() == ITEM_STATE_NWHIP)
					{
						isUpWhip = true;
					}
					f->isRemove = true;
				}
				if (isUpWhip==true)
				{
					if (whip->GetState()==WHIP_STATE_WHITE)
					{
						whip->SetState(WHIP_STATE_BLUE);
					}
					else if (whip->GetState() == WHIP_STATE_BLUE)
					{
						whip->SetState(WHIP_STATE_RED);
					}
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
					
				}
				if (e->ny!=0)// xét nếu va chạm theo phương thẳng đứng
				{
					y += dy;// vẫn cho simon rơi xuống 
				}
				
			}

			 if (dynamic_cast<CDagger *>(e->obj))
			{
				if (e->nx!=0)
				{
					x += dx;
				}
				if (e->ny!=0)
				{
					y += dy;
				}
			}

			//Va cham voi ghost
			 if (dynamic_cast<CGhoul *>(e->obj))
			{
				CGhoul *goomba = dynamic_cast<CGhoul *>(e->obj);
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
				}
				else if (e->nx != 0)
				{
	
				}
			}

		
		}
	}

	//check 
	bool colwithStair = false;
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<CItem *>(e))
		{
			CItem * f = dynamic_cast<CItem*> (e);

			if (CGameObject::isColliding(this, f) == true)
			{
			//	DebugOut(L"Co va cham \n");
				// thuc ra chi ngung render neu va cham chua remove han can toi uu
				//xong
			//	f->isRemove = true;
			}
		}
		if (dynamic_cast<CHiddenObjects *>(e))
		{
			
			CHiddenObjects * f = dynamic_cast<CHiddenObjects*> (e);
			if (f->GetState()==HO_STATE_STAIR_BOTTOM)
			{
				if (CGameObject::isColliding(this, f) == true)
				{
					bottomStair = true;
					topStair = false;
					colwithStair = true;
				}
				else
				{
					bottomStair = false;
				}
			}
			if (f->GetState() == HO_STATE_STAIR_TOP)
			{
				if (CGameObject::isColliding(this, f) == true)
				{
					topStair = true;
					colwithStair = true;
				}
				else
				{
					topStair = false;
				}
			}
			
		}
		if (colwithStair==true)
		{
			break;
		}
	}

	nHeart = nHeart < 0 ? 0: nHeart;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CSIMON::Render()
{
	
	int ani;
	if (state == SIMON_STATE_UP_STAIR)
	{
		ani = SIMON_ANI_UPSTAIR;
		animations[ani]->Render(nx, x, y, 255);
		RenderBoundingBox(x + 14, y);
		return;
	}
	if (state==SIMON_STATE_IDLE_UP_STAIR)
	{
		ani = SIMON_ANI_IDLE_UPSTAIR;
		animations[ani]->Render(nx, x, y, 255);
		RenderBoundingBox(x + 14, y);
		return;
	}
	if (state== SIMON_STATE_DOWN_STAIR)
	{
		ani = SIMON_ANI_DOWNSTAIR;
		animations[ani]->Render(nx, x, y, 255);
		RenderBoundingBox(x + 14, y);
		return;
	}
	if (state == SIMON_STATE_IDLE_DOWN_STAIR)
	{
		ani = SIMON_ANI_IDLE_DOWNSTAIR;
		animations[ani]->Render(nx, x, y, 255);
		RenderBoundingBox(x + 14, y);
		return;
	}

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	{
		//check Simon sits
		if (isJump == true)
		{
				ani = SIMON_ANI_SIT_JUMP;				
				
		}
		else
		{
			if (vx!=0)
			{
				if (state == SIMON_STATE_STAND_FIGHTING)
				{
					//DebugOut(L"simon fi");
					ani = SIMON_ANI_STAND_FIRE;
				}
				else
				{
					ani = SIMON_ANI_WALKING;
				}
			}
			else
			{
				if (state == SIMON_STATE_STAND_FIGHTING)
				{
					//DebugOut(L"simon fi");
					ani = SIMON_ANI_STAND_FIRE;
				}
				else
				{
					ani = SIMON_ANI_IDLE;
				}
			}

		}
	}

	// Xu ly whip
	if (state == SIMON_STATE_STAND_FIGHTING )
	{
	
		whip->setnx(nx);
		
		if (isSitting==true)
		{
			ani = SIMON_ANI_SIT_FIRE;
			whip->SetPosition(x - 85, y+12);

		}
		else
		{
			whip->SetPosition(x - 85, y);
		}
		if (isDelay == false)
		{
			whip->Render();
		}
	
	}
	else
	{
		//reset current frame

		if (whip->GetState()==WHIP_STATE_RED)
		{
			CAnimations::GetInstance()->Get(557)->setCurrentFrame(-1);
		}
		else if (whip->GetState() == WHIP_STATE_WHITE)
		{
			CAnimations::GetInstance()->Get(555)->setCurrentFrame(-1);
		}
		else
		{
			CAnimations::GetInstance()->Get(556)->setCurrentFrame(-1);
		}
	}
		


	if (state==SIMON_STATE_UPWHIP)
	{
		ani = SIMON_ANI_UPWHIP;
		DebugOut(L"Ani running \n");
	}


	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[ani]->Render(nx,x, y,alpha);
	// show boundingbox de check va cham
	RenderBoundingBox(x+14,y);
	
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

		DebugOut(L"[Line]:%d SIMON state jump", __LINE__);
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_UPWHIP:
		if (CAnimations::GetInstance()->Get(507)->getCurrentFrame()==4)
		{
			state = SIMON_STATE_IDLE;
			isUpWhip = false;
			//reset current frame
			CAnimations::GetInstance()->Get(507)->setCurrentFrame(-1);
		}
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_STAND_FIGHTING:
		if (isJump==false)
		{
			vx = 0;	// dung khi simon dung vampie killer
		}
		break;
	case SIMON_STATE_UP_STAIR:
	{
		isUpStair = true;
		if (nx>0)
		{
			vx = 0.06;
		}
		else if (nx<0)
		{
			vx = -0.06;
		}
		vy = -0.06;
		break;
	}
	case SIMON_STATE_DOWN_STAIR:
	{
		if (isUpStair==true)
		{
			nx = -nx;
			isUpStair = false;
		}
		isDownStair = true;
		if (nx>0)
		{
			vx = +0.06;
		}
		else if (nx<0)
		{
			vx = -0.06;
		}
		vy = 0.06;
		break;
	}
	case SIMON_STATE_IDLE_UP_STAIR:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_IDLE_DOWN_STAIR:
		vx = 0;
		vy = 0;
		break;
	}
}

//get bounding box dùng để xét va chạm vì khi vẽ sprite lên khung hình thường
//to hơn hình ảnh bên trong,
void CSIMON::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//x y của SIMON sẽ xét lại
	left = x+14;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH+14;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;
}


void CSIMON::LoadResource()
{
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"Resource\\sprites\\Simon\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	// big
	sprites->Add(10001, 28 + 27 - 30, 3, 28 + 27 + 30, 64, texSimon);		// idle right
	sprites->Add(10002, 92 - 30 + 27, 3, 92 + 30 + 27, 64, texSimon);		// walk
	sprites->Add(10003, 150 - 30 + 27, 3, 150 + 30 + 27, 64, texSimon);
	sprites->Add(10004, 210 - 30 + 27, 3, 210 + 30 + 27, 64, texSimon);

	sprites->Add(10016, 268 - 31 + 27, 3, 268 + 31 + 27, 64, texSimon);//Simon sit Right


	sprites->Add(10017, 356 - 30, 3, 356 + 30, 64, texSimon);//Simon stand fight
	sprites->Add(10018, 427 - 30, 3, 427 + 30, 64, texSimon);
	sprites->Add(10019, 501 - 30, 3, 501 + 30, 64, texSimon);



	sprites->Add(10021, 450 - 30 + 27, 67, 450 + 30 + 27, 130, texSimon);//Simon sit fight
	sprites->Add(10022, 28 - 30 + 27, 134, 28 + 30 + 27, 196, texSimon);
	sprites->Add(10023, 88 - 30 + 27, 134, 88 + 30 + 27, 196, texSimon);



	sprites->Add(10051, 58 - 30, 198, 58 + 30, 264, texSimon);//Simon Up whip
	sprites->Add(10052, 90 - 30 + 27, 198, 90 + 30 + 27, 264, texSimon);
	sprites->Add(10053, 150 - 30 + 27, 198, 150 + 30 + 27, 264, texSimon);
	sprites->Add(10054, 210 - 30 + 27, 198, 210 + 30 + 27, 264, texSimon);


	//simon on stair
	sprites->Add(10055, 173-30 , 67, 173+30, 130, texSimon);
	sprites->Add(10056, 230-30 , 67, 230 + 30, 130, texSimon);
	sprites->Add(10057, 291-30 , 67, 291 + 30 , 130, texSimon);
	sprites->Add(10058, 353 - 30, 67, 353 + 30 , 130, texSimon);



	sprites->Add(10099, 215, 120, 231, 135, texSimon);		// die 







	LPANIMATION ani;


	ani = new CAnimation(150);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(150);	// walk right big
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(500, ani);

	ani = new CAnimation(100); //simon sit 
	ani->Add(10016);
	animations->Add(505, ani);


	ani = new CAnimation(WHIP_DELAY_TIME);//simon stand fight
	ani->Add(10017);
	ani->Add(10018);
	ani->Add(10019);
	ani->Add(10001);
	animations->Add(502, ani);



	ani = new CAnimation(WHIP_DELAY_TIME); //simon sit  fight
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10016);
	animations->Add(506, ani);



	ani = new CAnimation(100); //simon up whip
	ani->Add(10051);
	ani->Add(10052);
	ani->Add(10053);
	ani->Add(10054);
	ani->Add(10001);
	animations->Add(507, ani);

	ani = new CAnimation(SIMON_ONSTAIR_TIME); //simon on stair up
	ani->Add(10057);
	ani->Add(10058);
	ani->Add(10057);
	animations->Add(508, ani);

	ani = new CAnimation(SIMON_ONSTAIR_TIME); //simon on stair down
	ani->Add(10055);
	ani->Add(10056);
	ani->Add(10055);
	animations->Add(509, ani);

	ani = new CAnimation(150); //simon idle up stair
	ani->Add(10057);
	animations->Add(510, ani);


	ani = new CAnimation(150); //simon idle down stair
	ani->Add(10055);
	animations->Add(511, ani);




	ani = new CAnimation(100);		// SIMON die
	ani->Add(10099);
	animations->Add(599, ani);
}


