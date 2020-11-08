#pragma once
#include "GameObject.h"
#define BULLET_SPEED 0.1f

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_FLYING_SPACE 200

#define BULLET_STATE_FLYING_LEFT 1
#define BULLET_STATE_FLYING_RIGHT 2
#define BULLET_STATE_DIE 3

#define BULLET_ANI_FLYING 0


class CBullet : public CGameObject
{
	
	float startPositionX;
	float startPositionY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


public:
	CBullet(float l, float t, int k);
	virtual void SetState(int state);

};

