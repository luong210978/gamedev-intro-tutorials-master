#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "HERO.h"
#include "Game.h"
#include "Brick.h"
#include "Goomba.h"
#include "Portal.h"

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

void CHERO::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += HERO_GRAVITY*dt;

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back HERO if collides with a moving objects, what if HERO is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;
		if (nx != 0) {
			vx = 0;
			
		}
		if (ny != 0) { vy = 0; this->setjump(1); }
		
		else this->setjump(0);
	
		
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -HERO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
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
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId(),p->GetScenePlace());
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* p = dynamic_cast<CBrick*>(e->obj);
				if (p->type == 11)
					this->hp -= p->Getlosehp(p->type);
				if (p->type == 12)
				{
					CGame* game = CGame::GetInstance();
					this->isdown = 1; if ((game->IsKeyDown(DIK_DOWN)))
						this->SetPosition(p->x + 3, p->y);
					this->isdown = 1; if ((game->IsKeyDown(DIK_UP)))
						this->SetPosition(p->x + 3, p->y-36);
					this->SetState(HERO_STATE_DOWN);
					//this-> vx = 1;
					//animation_set->at(HERO_STATE_DOWN)->Render(Ge, y, 255);

					//RenderBoundingBox();
				}
				else this->isdown == 0;
				
				if (this->hp < 0)
					this->SetState(HERO_STATE_ONLYMANDIE);
			}							
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHERO::Render()
{
	int ani = -1;
	if (state == HERO_STATE_ONLYMANDIE)
		ani = HERO_ANI_ONLYMANDIE;
	else
	{
		if (state == HERO_STATE_DOWN)
		{
 			ani = HERO_ANI_DOWN;
		}
		else
			if (level == HERO_LEVEL_ONLYMAN)
			{
				if (vx == 0)
				{
					if (nx > 0) ani = HERO_ANI_ONLYMAN_IDLE_RIGHT;
					else ani = HERO_ANI_ONLYMAN_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = HERO_ANI_ONLYMAN_WALKING_RIGHT;
				else ani = HERO_ANI_ONLYMAN_WALKING_LEFT;

			}
			else if (level == HERO_LEVEL_INCAR)
			{
				if (vx == 0)
				{
					if (nx > 0) ani = HERO_ANI_INCAR_IDLE_RIGHT;
					else ani = HERO_ANI_INCAR_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = HERO_ANI_INCAR_WALKING_RIGHT;
				else ani = HERO_ANI_INCAR_WALKING_LEFT;
			}
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
	case HERO_STATE_DOWN:
		vy = 0;
		break;
	case HERO_STATE_WALKING_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_WALKING_LEFT: 
		vx = -HERO_WALKING_SPEED;
		nx = -1;
		break;
	case HERO_STATE_JUMP:
		// TODO: need to check if HERO is *current* on a platform before allowing to jump again
		vy = -HERO_JUMP_SPEED_Y;
		break; 
	case HERO_STATE_IDLE: 
		vx = 0;
		
		break;
	case HERO_STATE_ONLYMANDIE:
		vy = 0;
		//vy = -HERO_ONLYMANDIE_DEFLECT_SPEED;
		break;
	
	}
}

void CHERO::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==HERO_LEVEL_ONLYMAN)
	{
		right = x + HERO_ONLYMAN_BBOX_WIDTH;
		bottom = y + HERO_ONLYMAN_BBOX_HEIGHT;
	}
	else
	{
		right = x + HERO_INCAR_BBOX_WIDTH;
		bottom = y + HERO_INCAR_BBOX_HEIGHT;
	}
}

/*
	Reset HERO status to the beginning state of a scene
*/
void CHERO::Reset()
{
	SetState(HERO_STATE_IDLE);
	SetLevel(HERO_LEVEL_ONLYMAN);
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

		break;

	case 2:
		
		SetLevel(HERO_LEVEL_INCAR);
		SetPosition(this->x, this->y - 3);
		break;
	}
}

