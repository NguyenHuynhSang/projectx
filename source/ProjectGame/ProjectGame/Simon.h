#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED		0.15f 
#define SIMON_FIRE_TIME			300
//0.1f
#define SIMON_JUMP_SPEED_Y		0.54f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

//Simon State

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT             500
#define SIMON_STAGE_STAND_FIRE			600
#define SIMON_STAGE_SIT_FIRE			700


#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			1
#define SIMON_ANI_BIG_WALKING_RIGHT			2
#define SIMON_ANI_BIG_WALKING_LEFT			3
#define SIMON_ANI_SITorJump_LEFT            4
#define SIMON_ANI_SITorJump_RIGHT            5
#define SIMON_ANI_STAND_FIRE				6
#define SIMON_ANI_SIT_FIRE					7
#define SIMON_ANI_DIE				8

#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  30
#define SIMON_BIG_BBOX_HEIGHT 60
//
//#define SIMON_SMALL_BBOX_WIDTH  13
//#define SIMON_SMALL_BBOX_HEIGHT 15

#define SIMON_UNTOUCHABLE_TIME 5000


class CSIMON : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	DWORD setTimeforDelay;

public:
	bool isFighting = false;
	CSIMON() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	bool getStateforAniSitandJump=false;
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};