#pragma once
#include "GameObject.h"
#include "car.h"
//setmove
#define HERO_WALKING_SPEED		0.15f 
#define HERO_DOWN_WALKING_SPEED		0.1f 
//0.1f
#define HERO_JUMP_SPEED_Y	0.5f
#define HERO_JUMP_DEFLECT_SPEED	0.2f
#define HERO_GRAVITY			0.002f
#define HERO_GRAVITYdithang			0.0002f
#define HERO_ONLYMANDIE_DEFLECT_SPEED	 0.5f
//==========sate==================
#define HERO_STATE_IDLE			0
#define HERO_STATE_WALKING_RIGHT	100
#define HERO_STATE_WALKING_LEFT	200
#define HERO_STATE_JUMP	300
///////////////////////////////////////////
#define HERO_STATE_ONLYMANDIE	400
#define HERO_STATE_INCARDIE chưa làm xong
#define HERO_STATE_DOWN	500
/////////////////////////////////////////
#define HERO_STATE_UP_HALF 600
#define HERO_STATE_UP_HALF_WALKING_RIGHT	700
#define HERO_STATE_UP_HALF_WALKING_LEFT		800
#define HERO_SATE_JUMP_UP_HALF			900
////////////////////////////////////////////////////
#define HERO_STATE_JUMP_UP	1000
#define HERO_STATE_UP 1100
#define HERO_STATE_UP_WALKING_RIGHT 1300
#define HERO_STATE_UP_WALKING_LEFT	1400

////////////////////////////////////////
//==========ANI==============================
#define HERO_ANI_ONLYMAN_IDLE_LEFT			0
#define HERO_ANI_ONLYMAN_WALKING_LEFT		1
#define HERO_ANI_ONLYMAN_IDLE_RIGHT			2
#define HERO_ANI_ONLYMAN_WALKING_RIGHT		3
#define HERO_ANI_ONLYMAN_DIE				4
/////////////////////////////////////////////
#define HERO_ANI_ONLYMAN_DOWN1				5
#define HERO_ANI_ONLYMAN_DOWN2				6
//////////////////////////////////////////////
#define HERO_ANI_ONLYMAN_BO_LEFT1			7
#define HERO_ANI_ONLYMAN_BO_LEFT2			8
#define HERO_ANI_ONLYMAN_BO_RIGHT1			9
#define HERO_ANI_ONLYMAN_BO_RIGHT2			10
//////////////////////////////////////////////
#define HERO_ANI_INCAR_LEFT1				11
#define HERO_ANI_INCAR_LEFT2				12
#define HERO_ANI_INCAR_LEFT3				13
#define HERO_ANI_INCAR_LEFT4				14
//////////////////////////////////////////////
#define HERO_ANI_INCAR_RIGHT1				15
#define HERO_ANI_INCAR_RIGHT2				16
#define HERO_ANI_INCAR_RIGHT3				17
#define HERO_ANI_INCAR_RIGHT4				18
//////////////////////////////////////////////
#define HERO_ANI_INCAR_OUT_LEFT				19
#define HERO_ANI_INCAR_OUT_RIGHT			20
//////////////////////////////////////////////
#define HERO_ANI_INCAR_UP_HALF_LEFT1		21
#define HERO_ANI_INCAR_UP_HALF_LEFT2		22
#define HERO_ANI_INCAR_UP_HALF_LEFT3		23
#define HERO_ANI_INCAR_UP_HALF_LEFT4		24
//////////////////////////////////////////////
#define HERO_ANI_INCAR_UP_HALF_RIGHT1		25
#define HERO_ANI_INCAR_UP_HALF_RIGHT2		26
#define HERO_ANI_INCAR_UP_HALF_RIGHT3		27
#define HERO_ANI_INCAR_UP_HALF_RIGHT4		28
//////////////////////////////////////////////
#define HERO_ANI_INCAR_UP_LEFT1				29
#define HERO_ANI_INCAR_UP_LEFT2				30
#define HERO_ANI_INCAR_UP_LEFT3				31
#define HERO_ANI_INCAR_UP_LEFT4				32
//////////////////////////////////////////////
#define HERO_ANI_INCAR_UP_RIGHT1			33
#define HERO_ANI_INCAR_UP_RIGHT2			34
#define HERO_ANI_INCAR_UP_RIGHT3			35
#define HERO_ANI_INCAR_UP_RIGHT4			36
//////////////////////////////////////////////
#define	HERO_LEVEL_INCAR	1
#define	HERO_LEVEL_ONLYMAN	2
#define	HERO_LEVEL_DICAUTHANG	3
#define HERO_LEVEl_BO	4
//==============level=====================
#define HERO_ONLYMAN_BBOX_WIDTH  8
#define HERO_ONLYMAN_BBOX_HEIGHT 16
/////////////////////////////////////////
#define HERO_INCAR_BBOX_WIDTH  26
#define HERO_INCAR_BBOX_HEIGHT 16
/////////////////////////////////////////
#define HERO_BO_BBOX_WIDTH	16
#define HERO_BO_BBOX_HEIGHT 8
/////////////////////////////////////////
#define HERO_UNTOUCHABLE_TIME 3000
//////////////////////////////////////
#define HERO_HP = 100
//===========BULLET====================
#define HERO_ANIID_BULLET_ONLYMAN 22
#define HERO_ANIID_BULLET_ONLYMAN_NAM	23
#define HERO_ANIID_BULLET_INCAR 24
#define Thoi_gian_chuyen 100
#define Thoi_gian_chuyen2 2000
class CHERO : public CGameObject
{
	bool isjump;
	int level;
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of HERO at scene
	float start_y;
public:
	int bani = -1;
	int quaybanhxe = 1;
	int bbanhxe = 1;
	//DWORD tgchuyen;
	DWORD tgbdchuyen;
	DWORD tgquaybanhxe;

	int xe_left[4] = { HERO_ANI_INCAR_LEFT1 ,HERO_ANI_INCAR_LEFT2 ,HERO_ANI_INCAR_LEFT3 ,HERO_ANI_INCAR_LEFT4 };
	int xe_right[4] = { HERO_ANI_INCAR_RIGHT1 ,HERO_ANI_INCAR_RIGHT2,HERO_ANI_INCAR_RIGHT3,HERO_ANI_INCAR_RIGHT4 };
	int xe_left_up_half[4] = { HERO_ANI_INCAR_UP_HALF_LEFT1 ,HERO_ANI_INCAR_UP_HALF_LEFT2,HERO_ANI_INCAR_UP_HALF_LEFT3,HERO_ANI_INCAR_UP_HALF_LEFT4 };
	int xe_right_up_half[4] = { HERO_ANI_INCAR_UP_HALF_RIGHT1 ,HERO_ANI_INCAR_UP_HALF_RIGHT2,HERO_ANI_INCAR_UP_HALF_RIGHT3,HERO_ANI_INCAR_UP_HALF_RIGHT4 };
	int xe_left_up[4] = { HERO_ANI_INCAR_UP_LEFT1 ,HERO_ANI_INCAR_UP_LEFT2,HERO_ANI_INCAR_UP_LEFT3,HERO_ANI_INCAR_UP_LEFT4 };
	int xe_right_up[4] = { HERO_ANI_INCAR_UP_RIGHT1 ,HERO_ANI_INCAR_UP_RIGHT2,HERO_ANI_INCAR_UP_RIGHT3,HERO_ANI_INCAR_UP_RIGHT4 };
	int ani = -1;
	float crw, crh;
	Ccar* car;
	float hp;
	bool getinto,isup = false;
	CHERO(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
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
	void Startchuyen() {
		tgbdchuyen = GetTickCount();
	}
	bool Tinhtoanchuyen1() { if (GetTickCount() - tgbdchuyen >= Thoi_gian_chuyen) return true; return false; }
	bool Tinhtoanchuyen2() { if (GetTickCount() - tgbdchuyen >= Thoi_gian_chuyen2) return true; return false; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};