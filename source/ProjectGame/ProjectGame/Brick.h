#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  30
#define BRICK_BBOX_HEIGHT 30



class CBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void LoadResource();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	CBrick();
};