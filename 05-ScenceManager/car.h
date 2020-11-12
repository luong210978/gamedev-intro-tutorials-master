#pragma once
#pragma once
#include "GameObject.h"
#define CAR_SPEED 0.1f
#define HERO_ANI_INCAR_OUT_LEFT				19
#define HERO_ANI_INCAR_OUT_RIGHT			20

#define CAR_BBOX_WIDTH 26
#define CAR_BBOX_HEIGHT 26

#define CAR_FLYING_SPACE 200

#define CAR_STATE_FLYING_LEFT 2
#define CAR_STATE_FLYING_RIGHT 3
#define CAR_STATE_DIE 3

#define CAR_ANI_FLYING 0


class Ccar: public CGameObject
{

	float startPositionX;
	float startPositionY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


public:
	Ccar(float l, float t, int k);
	virtual void Setn(int n);
};

