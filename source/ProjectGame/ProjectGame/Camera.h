#pragma once
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"Global.h"

#define CAM_AUTO_TIME 2000
#define CAM_AUTO_SPEED 0.5f
class CCamera
{
	float CamX;
	float CamY;
	DWORD startautoTime;
	DWORD endautoTime;
public:
	bool isauto = false;
	bool camStop = false;
	static CCamera * __instance;
	void setCamera(float x, float y) { CamX = x; CamY = y; };
	void getCamera(float &x, float &y) { x = CamX; y = CamY; };
	float getCamX() { return CamX; }
	static CCamera * GetInstance();
	void ResetCam() { CamX = 0; CamY = 0; };
	bool autoCamera(float X);
	float getVPWidth() { return CamX + SCREEN_WIDTH; }
	float getVPHeight() { return CamY + SCREEN_HEIGHT; }
	bool isCamMove;
	CCamera();
	~CCamera();
};

