﻿#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include"Game.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};



class CGameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	int state;

	DWORD dt;

	vector<LPANIMATION> animations;


	
public:
	bool isRemove = false;

	void SetPosition(float x, float y) { this->x = x, this->y = y;}
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetState() { return this->state; }
	

	//AABB: kiểm tra xem 2 cái boundingbox có chồng nhau hay không , nếu có là có va chạm
	// dùng để xét va chạm khi vx=0
	bool isColliding(CGameObject *a,CGameObject *b) {
		float top, left, right, button;
		a->GetBoundingBox(left, top, right, button);
		float l, t, r, bt;
		b->GetBoundingBox(l, t, r, bt);
		return AABB(l, t, r, bt, left, top, right, button);
	}
	// thuat toan xu ly va cham AABB
	bool AABB(float l, float t, float r, float b, float l1, float t1, float r1, float b1);




	void RenderBoundingBox(float a,float b);
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);
	void AddAnimation(int aniId);

	CGameObject();
	virtual void LoadResource()=0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	//load resource
	//virtual void LoadResource() = 0;
	~CGameObject();
};

