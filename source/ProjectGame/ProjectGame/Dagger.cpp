﻿#include "Dagger.h"
#include"Camera.h"
#include"Global.h"
#include"Torch.h"
#include"debug.h"
void CDagger::Render()
{

		animations[state]->Render(-nx, x, y);
		RenderBoundingBox(x,y);
	
}

void CDagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* co)
{
	if (x<0||x>1535)
	{
		isRemove = true;
		return;
	}
	vx = DAGGER_SPEED;
	if (nx<0)
	{
		vx = -vx;
	}
	
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	coEventsResult.clear();
	CalcPotentialCollisions(co, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		bool isre = false;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//xu ly va cham 
			if (dynamic_cast<CTorch *>(e->obj))
			{
				CTorch * f = dynamic_cast<CTorch*> (e->obj);
				if (e->nx!=0)
				{
					f->SetState(TORCH_STATE_DISAPPEAR);
					//DebugOut(L"\nChay vao day");
					this->isRemove = true;
					isre = true;
				}
	
			}
			if (isre==true)
			{
				break;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

















	//for (int i = 0; i < co->size(); i++)
	//{
	//	if (this->isRemove==true)
	//	{
	//		break;
	//	}
	//	LPGAMEOBJECT e = co->at(i);
	//	if (dynamic_cast<CTorch *>(e))
	//	{
	//		CTorch * f = dynamic_cast<CTorch*> (e);
	//		if (CGameObject::isColliding(this, f) == true)
	//		{

	//			DebugOut(L"Co va cham \n");
	//			f->SetState(TORCH_STATE_DISAPPEAR);
	//		
	//		}
	//	}
	//}
	//// clean up collision events
}

CDagger::CDagger()
{
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	CTextures * textures = CTextures::GetInstance();	
	LPDIRECT3DTEXTURE9 texDanger = textures->Get(202);
	sprites->Add(70001, 0, 0, 24, 20, texDanger);
	LPANIMATION ani;

	ani = new CAnimation(100);		// danger
	ani->Add(70001);
	animations->Add(902, ani);
	this->AddAnimation(902);
	//carefull
	//item->GetPosition(x, y);
	//thêm vào object mới có thể update, hay bắt sự kiện va chạm
	//vì cái coobject dc lay ra tu object
	//k thêm vào lấy đâu ra mà xét.
	CGlobal::GetInstance()->objects.push_back(this);
}


CDagger::~CDagger()
{
}
