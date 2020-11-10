#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "HERO.h"
#include "Game.h"
#include "Brick.h"
#include "Goomba.h"
#include "Portal.h"
#include "car.h"
CHERO::CHERO(float x, float y) : CGameObject()
{
	level = HERO_LEVEL_ONLYMAN;
	untouchable = 0;
	SetState(HERO_STATE_IDLE);
	isjump = false;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	hp = 100;
}

void CHERO::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	vy += HERO_GRAVITY * dt;
	float x1, y1;
	car->GetPosition(x1, y1);
	if ((x > CAR_BBOX_WIDTH + x1) || (y > CAR_BBOX_HEIGHT + y1) || (x < x1)||(y< y1))
		getinto = false;
	else
		getinto = true;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when ONLYMANDIE 
	if (state!=HERO_STATE_ONLYMANDIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > HERO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if (nx != 0) {
			vx = 0;			
		}
		if (ny != 0) { vy = 0;}
		else
		this->setjump(0);
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			 if (dynamic_cast<Ccar*>(e->obj))//xe 
			{
				this->SetSpeed(HERO_WALKING_SPEED, HERO_GRAVITY);
				this->getinto = true;
				x += dx;
				if (coEvents.size() == 1)
					y += dy;
			}			
			else //anything else;
			{
#pragma region quái
				if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -HERO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level > HERO_LEVEL_INCAR)
								{
									level = HERO_LEVEL_INCAR;
									StartUntouchable();
								}
								else
									SetState(HERO_STATE_ONLYMANDIE);
							}
						}
					}
				} // if Goomba
				else if (dynamic_cast<CPortal*>(e->obj))
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId(), p->GetScenePlace(),p->GetSceneLevel());
				}
#pragma endregion
				else if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick* p = dynamic_cast<CBrick*>(e->obj);
					CGame* game = CGame::GetInstance();
#pragma region gach lua					
					if (p->type == 11)
						this->hp -= p->Getlosehp(p->type);
					if (this->hp < 0)
						this->SetState(HERO_STATE_ONLYMANDIE);
#pragma endregion
#pragma region di cau thang
					if (p->type == 12)
					{
						if (level == HERO_LEVEL_ONLYMAN)
						{
							SetLevel(HERO_LEVEL_DICAUTHANG);
						}
						if (level == HERO_LEVEL_DICAUTHANG)
						{
							if ((game->IsKeyDown(DIK_DOWN)))
							{
								this->SetPosition(p->x + 3, p->y);
								if (bani == HERO_ANI_DOWN1)
									bani = HERO_ANI_DOWN2;
								else bani = HERO_ANI_DOWN1;

							}
							if ((game->IsKeyDown(DIK_UP)))
							{
								if (bani == HERO_ANI_DOWN1)
									bani = HERO_ANI_DOWN2;
								else bani = HERO_ANI_DOWN1;
								this->SetPosition(p->x + 3, p->y - 33);
							}
						}
					}
					else { if (level == HERO_LEVEL_DICAUTHANG) SetLevel(HERO_LEVEL_ONLYMAN); }
#pragma  endregion 
#pragma bò
					if (level == HERO_LEVEL_ONLYMAN)  
						if ((game->IsKeyDown(DIK_DOWN)))
						{
							this->SetLevel(HERO_LEVEl_BO);
							isjump = false;
						}					
					if (level == HERO_LEVEl_BO)
					{
						if ((game->IsKeyDown(DIK_UP)))
						{
							this->SetLevel(HERO_LEVEL_ONLYMAN);
							this->SetPosition(x, y - 10);
							isjump = true;
						}
					}
						else this->setjump(1);
#pragma endregion
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHERO::Render()
{
	ani = -1;
	CGame* game = CGame::GetInstance();
	if (state == HERO_STATE_ONLYMANDIE)
	{
		ani = HERO_ANI_ONLYMANDIE;
		return;
	}
	switch (level)
	{
	
	case HERO_LEVEL_DICAUTHANG:
		if (bani == HERO_ANI_DOWN1)
			ani = HERO_ANI_DOWN2;	
		else
			ani = HERO_ANI_DOWN1;
		break;
	case HERO_LEVEL_ONLYMAN:
		if (vx == 0)
		{
			if (nx > 0) ani = HERO_ANI_ONLYMAN_IDLE_RIGHT;
			else ani = HERO_ANI_ONLYMAN_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = HERO_ANI_ONLYMAN_WALKING_RIGHT;
		else ani = HERO_ANI_ONLYMAN_WALKING_LEFT;
		break;
	case HERO_LEVEL_INCAR:
		switch (state)
		{
		case HERO_STATE_UP:
			if (nx > 0)
				ani = HERO_ANI_INCAR_UP_RIGHT;
			else ani = HERO_ANI_INCAR_UP_LEFT;
			break;
		case HERO_STATE_UP_WALKING_RIGHT:
				ani = HERO_ANI_INCAR_UP_RIGHT;
			break;
		case HERO_STATE_UP_WALKING_LEFT:
			ani = HERO_ANI_INCAR_UP_LEFT;
			break;
		case HERO_STATE_JUMP:
			if (nx > 0)
				ani = HERO_ANI_INCAR_JUMP_RIGHT;
			else ani = HERO_ANI_INCAR_JUMP_LEFT;
				break;
		/*case HERO_STATE_IDLE:
			break;*/
		case HERO_STATE_JUMP_UP:
			if (nx > 0)
				ani = HERO_ANI_INCAR_UP_RIGHT;
			else ani = HERO_ANI_INCAR_UP_LEFT;
			break;
		case HERO_STATE_WALKING_LEFT:
			ani = HERO_ANI_INCAR_WALKING_LEFT;
			break;
		case HERO_STATE_WALKING_RIGHT:
			ani = HERO_ANI_INCAR_WALKING_RIGHT;
			break;
		default:
			if (nx > 0) ani = HERO_ANI_INCAR_IDLE_RIGHT;
			else ani = HERO_ANI_INCAR_IDLE_LEFT;
			break;
		}
		break;
	case HERO_LEVEl_BO:
		if (nx > 0)
		{
			if (bani == HERO_ANI_BO_RIGHT1)			
				ani = HERO_ANI_BO_RIGHT2;							
			else	
				ani = HERO_ANI_BO_RIGHT1;
		}
		else 
		{
			if (bani == HERO_ANI_BO_LEFT1)
				ani = HERO_ANI_BO_LEFT2;				
			else
				ani = HERO_ANI_BO_LEFT1;
		}
		break;
	default:
		break;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}
bool CHERO::getjump()
{
	return this->isjump;
}
void CHERO::setjump(bool isjump)
{
	 this->isjump=isjump;
}
void CHERO::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case HERO_STATE_WALKING_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_UP_WALKING_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_WALKING_LEFT: 
		vx = -HERO_WALKING_SPEED;
		nx = -1;
		break;
	case HERO_STATE_UP_WALKING_LEFT:
		vx = -HERO_WALKING_SPEED;
		nx = -1;
		break;
	case HERO_STATE_JUMP:
		// TODO: need to check if HERO is *current* on a platform before allowing to jump again
		vy = -HERO_JUMP_SPEED_Y;
		break; 
	case HERO_STATE_JUMP_UP:
		vy = -HERO_JUMP_SPEED_Y;
		break;
	case HERO_STATE_IDLE: 
		vx = 0;	
		break;
	case HERO_STATE_UP:
		vx = 0;
		break;
	case HERO_STATE_ONLYMANDIE:
		vy = 0;
		break;

	
	}
}

void CHERO::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	switch (level)
	{
	case HERO_LEVEL_ONLYMAN:

		right = x + HERO_ONLYMAN_BBOX_WIDTH;
		bottom = y + HERO_ONLYMAN_BBOX_HEIGHT;
		break;
	case HERO_LEVEL_INCAR:
		switch (state)
		{
		default:right = x + HERO_INCAR_BBOX_WIDTH;
			bottom = y + HERO_INCAR_BBOX_HEIGHT;
			break;
		
		case HERO_STATE_UP:
			right = x + 26;
			bottom = y + 34;
			break;
		case HERO_STATE_JUMP_UP:
			right = x + 26;
			bottom = y + 34;
			break;
		case HERO_STATE_UP_WALKING_RIGHT:
			right = x + 26;
			bottom = y + 34;
			break;
		case HERO_STATE_UP_WALKING_LEFT:
			right = x + 26;
			bottom = y + 34;
			break;
		}
		break;
	case HERO_LEVEL_DICAUTHANG:

		right = x + HERO_ONLYMAN_BBOX_WIDTH;
		bottom = y + HERO_ONLYMAN_BBOX_HEIGHT;
		break;
	case HERO_LEVEl_BO:

		right = x + HERO_BO_BBOX_WIDTH;
		bottom = y + HERO_BO_BBOX_HEIGHT;
		break;
	}
	
}

/*
	Reset HERO status to the beginning state of a scene
*/
void CHERO::Reset()
{
	SetState(HERO_STATE_IDLE);
	SetLevel(HERO_LEVEL_INCAR);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	this->hp = 100;
	
}
void CHERO::Changelevel()
{
	switch (level)
	{
	case 1:

		SetLevel(HERO_LEVEL_ONLYMAN);
		SetPosition(this->x + 6, this->y );

		break;

	case 2:
		
		SetLevel(HERO_LEVEL_INCAR);
		SetPosition(this->x-4, this->y-3);
		break;
	}
}

