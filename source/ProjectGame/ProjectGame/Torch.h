#pragma once
#include"GameObject.h"


#define TORCH_BBOX_WIDTH  30
#define TORCH_BBOX_HEITH  60

#define TORCH_STATE_BURNING 100
#define TORCH_STATE_BURNING 200

#define TORCH_ANI_BURNING 0
#define TORCH_ANI_DESTROY 1


class CTorch:public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();


public:
	void setNumOfTorchPerScene();
	virtual void SetState(int state);
	/*CTorch();
	~CTorch();*/
};
