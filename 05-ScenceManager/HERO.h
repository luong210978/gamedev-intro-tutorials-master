#pragma once
#include "GameObject.h"

#define HERO_WALKING_SPEED		0.15f 
#define HERO_DOWN_WALKING_SPEED		0.1f 
//0.1f
#define HERO_JUMP_SPEED_Y	0.5f
#define HERO_JUMP_DEFLECT_SPEED	0.2f
#define HERO_GRAVITY			0.002f
#define HERO_ONLYMANDIE_DEFLECT_SPEED	 0.5f

#define HERO_STATE_IDLE			0
#define HERO_STATE_WALKING_RIGHT	100
#define HERO_STATE_WALKING_LEFT	200
#define HERO_STATE_JUMP			300
#define HERO_STATE_ONLYMANDIE				400

#define HERO_ANI_ONLYMAN_IDLE_RIGHT	0
#define HERO_ANI_ONLYMAN_IDLE_LEFT		1
#define HERO_ANI_INCAR_IDLE_RIGHT		2
#define HERO_ANI_INCAR_IDLE_LEFT		3

#define HERO_ANI_ONLYMAN_WALKING_RIGHT		4
#define HERO_ANI_ONLYMAN_WALKING_LEFT		5
#define HERO_ANI_INCAR_WALKING_RIGHT	6
#define HERO_ANI_INCAR_WALKING_LEFT	7

#define HERO_ANI_ONLYMANDIE				8

#define	HERO_LEVEL_INCAR	1
#define	HERO_LEVEL_ONLYMAN	2

#define HERO_ONLYMAN_BBOX_WIDTH  8
#define HERO_ONLYMAN_BBOX_HEIGHT 16

#define HERO_INCAR_BBOX_WIDTH  26
#define HERO_INCAR_BBOX_HEIGHT 18

#define HERO_UNTOUCHABLE_TIME 5000

#define HERO_HP = 100;
class CHERO : public CGameObject
{
	bool isjump;
	int level;
	float hp;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of HERO at scene
	float start_y; 
public: 
	CHERO(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	bool getjump();
	void setjump(bool isjump);
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void ChangeState();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};