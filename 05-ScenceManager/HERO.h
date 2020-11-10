#pragma once
#include "GameObject.h"
#include "car.h"
#define HERO_WALKING_SPEED		0.15f 
#define HERO_DOWN_WALKING_SPEED		0.1f 
//0.1f
#define HERO_JUMP_SPEED_Y	0.5f
#define HERO_JUMP_DEFLECT_SPEED	0.2f
#define HERO_GRAVITY			0.002f
#define HERO_GRAVITYdithang			0.0002f
#define HERO_ONLYMANDIE_DEFLECT_SPEED	 0.5f

#define HERO_STATE_IDLE			0
#define HERO_STATE_WALKING_RIGHT	100
#define HERO_STATE_WALKING_LEFT	200
#define HERO_STATE_JUMP	300
#define HERO_STATE_ONLYMANDIE	400

#define HERO_STATE_DOWN	500
#define HERO_STATE_JUMP_UP	600
#define HERO_STATE_UP 700
#define HERO_STATE_UP_WALKING_RIGHT 800
#define HERO_STATE_UP_WALKING_LEFT	900


#define HERO_ANI_ONLYMAN_IDLE_RIGHT	0
#define HERO_ANI_ONLYMAN_IDLE_LEFT		1
#define HERO_ANI_INCAR_IDLE_RIGHT		2
#define HERO_ANI_INCAR_IDLE_LEFT		3

#define HERO_ANI_ONLYMAN_WALKING_RIGHT		4
#define HERO_ANI_ONLYMAN_WALKING_LEFT		5
#define HERO_ANI_INCAR_WALKING_RIGHT	6
#define HERO_ANI_INCAR_WALKING_LEFT	7

#define HERO_ANI_ONLYMANDIE				8

#define HERO_ANI_DOWN1	9
#define HERO_ANI_DOWN2	10

#define HERO_ANI_BO_LEFT1	11
#define HERO_ANI_BO_LEFT2	12

#define HERO_ANI_BO_RIGHT1	13
#define HERO_ANI_BO_RIGHT2	14

#define HERO_ANI_INCAR_JUMP_LEFT 15
#define HERO_ANI_INCAR_JUMP_RIGHT 16

#define HERO_ANI_INCAR_UP_LEFT 17
#define HERO_ANI_INCAR_UP_RIGHT 18

#define	HERO_LEVEL_INCAR	1
#define	HERO_LEVEL_ONLYMAN	2
#define	HERO_LEVEL_DICAUTHANG	3
#define HERO_LEVEl_BO	4

#define HERO_ONLYMAN_BBOX_WIDTH  8
#define HERO_ONLYMAN_BBOX_HEIGHT 16

#define HERO_INCAR_BBOX_WIDTH  26
#define HERO_INCAR_BBOX_HEIGHT 18

#define HERO_BO_BBOX_WIDTH	16
#define HERO_BO_BBOX_HEIGHT 8

#define HERO_UNTOUCHABLE_TIME 5000

#define HERO_HP = 100
#define HERO_ANIID_BULLET_ONLYMAN 22
#define HERO_ANIID_BULLET_ONLYMAN_NAM	23
#define HERO_ANIID_BULLET_INCAR 24

class CHERO : public CGameObject
{
	bool isjump;
	//bool isban;
	int level;
	
	
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of HERO at scene
	float start_y; 
public: 
	int bani = -1;
	int ani = -1;
	float crw, crh;
	Ccar* car;
	float hp;
	bool getinto=false;
	CHERO(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	bool getjump();
	void setjump(bool isjump);
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel()
	{
		return level;
	}
	void Changelevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};