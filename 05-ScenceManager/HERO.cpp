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
	level = start_level;
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
	if (level != HERO_LEVEL_OVH)
	{
		vy += HERO_GRAVITY * dt;

		if (vx != 0)
		{
			if (GetTickCount() - tgquaybanhxe >= Thoi_gian_chuyen)
			{
				quaybanhxe += 1;
				if (level == HERO_LEVEL_INCAR)
				{
					if (isup)
					{
						if (nx == 1)
							if ((state != HERO_STATE_UP_HALF_WALKING_RIGHT) && (state != HERO_STATE_UP_WALKING_RIGHT))
							{
								SetState(HERO_STATE_UP_HALF_WALKING_RIGHT);
								SetPosition(x, y - 14);
							}
							else
								if ((state != HERO_STATE_UP_WALKING_RIGHT) && (state == HERO_STATE_UP_HALF_WALKING_RIGHT))
								{
									SetPosition(x, y - 5);
									SetState(HERO_STATE_UP_WALKING_RIGHT);
								}
						if (nx == -1)
							if ((state != HERO_STATE_UP_HALF_WALKING_LEFT) && (state != HERO_STATE_UP_WALKING_LEFT))
							{
								SetState(HERO_STATE_UP_HALF_WALKING_LEFT);
								SetPosition(x, y - 14);
							}
							else
								if ((state != HERO_STATE_UP_WALKING_LEFT) && (state == HERO_STATE_UP_HALF_WALKING_LEFT))
								{
									SetPosition(x, y - 5);
									SetState(HERO_STATE_UP_WALKING_LEFT);
								}

					}
					if (!isup)
					{
						if (nx == 1)
							if (state == HERO_STATE_UP_WALKING_RIGHT)
							{
								SetState(HERO_STATE_UP_HALF_WALKING_RIGHT);
								SetPosition(x, y + 3);

							}
							else
								if (state = HERO_STATE_UP_HALF_WALKING_RIGHT)
								{
									SetState(HERO_STATE_WALKING_RIGHT);
								}
						if (nx == -1)
							if (state == HERO_STATE_UP_WALKING_LEFT)
							{
								SetState(HERO_STATE_UP_HALF_WALKING_LEFT);
								SetPosition(x, y + 3);

							}
							else
								if (state == HERO_STATE_UP_HALF_WALKING_LEFT)
								{
									SetState(HERO_STATE_WALKING_LEFT);
								}

					}
					quaybanhxe = quaybanhxe % 4;
					tgquaybanhxe = GetTickCount();
				}
				if (level == HERO_LEVEL_BO)
				{
					quaybanhxe = quaybanhxe % 2;
					tgquaybanhxe = GetTickCount();
				}
			}
		}
		if (vx == 0)
			if (isup)
			{
				if (GetTickCount() - tgbdchuyen >= Thoi_gian_chuyen)
					if (state == HERO_STATE_IDLE)
					{
						SetState(HERO_STATE_UP_HALF);
						SetPosition(x, y - 14);

						tgbdchuyen = GetTickCount();
					}
					else
						if (state == HERO_STATE_UP_HALF)
						{
							SetState(HERO_STATE_UP);
							SetPosition(x, y - 5);
						}
			}
			else
			{
				if (GetTickCount() - tgbdchuyen >= Thoi_gian_chuyen)
					if (state == HERO_STATE_UP)
					{
						SetState(HERO_STATE_UP_HALF);
						SetPosition(x, y + 4);
						tgbdchuyen = GetTickCount();
					}
					else
						if (state == HERO_STATE_UP_HALF)
						{
							SetState(HERO_STATE_IDLE);
							SetPosition(x, y + 10);
						}
			}
		if (car->visible)
		{
			float x1, y1;
			car->GetPosition(x1, y1);
			if ((x > CAR_BBOX_WIDTH + x1) || (y > CAR_BBOX_HEIGHT + y1) || (x+16 < x1) || (y < y1))
				getinto = false;
			else
				getinto = true;
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//////////////////////////////unstop/////////////////////////////////
	/*if ( GetTickCount() - untouchable_start > HERO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/
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
			if(level==HERO_LEVEL_ONLYMAN)
			 if (dynamic_cast<Ccar*>(e->obj))//xe 
			{
				this->SetSpeed(HERO_WALKING_SPEED, HERO_GRAVITY);
				this->getinto = true;
				x += dx;
				if (coEvents.size() == 1)
					y += dy;
			}			
			
			//if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0)
			//	{
			//		if (goomba->GetState() != GOOMBA_STATE_DIE)
			//		{
			//			goomba->SetState(GOOMBA_STATE_DIE);
			//			vy = -HERO_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable == 0)
			//		{
			//			if (goomba->GetState() != GOOMBA_STATE_DIE)
			//			{
			//				if (level > HERO_LEVEL_INCAR)
			//				{
			//					level = HERO_LEVEL_INCAR;
			//					StartUntouchable();
			//				}
			//				else
			//					SetState(HERO_STATE_ONLYMANDIE);
			//			}
			//		}
			//	}
			//} // if Goomba
			/*else*/ if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId(), p->GetScenePlace(), p->GetSceneLevel());
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* p = dynamic_cast<CBrick*>(e->obj);
				CGame* game = CGame::GetInstance();
				if (p->type == 11)
				{
					this->hp -= p->Getlosehp(p->type);
					if (this->hp < 0)
						this->SetState(HERO_STATE_ONLYMANDIE);
				}

				if (p->type == 12)


				{
					x = x + dx;
					if (level == HERO_LEVEL_ONLYMAN)
					{
						SetLevel(HERO_LEVEL_DICAUTHANG);
					}
					/*else 
					if(level!= HERO_LEVEL_DICAUTHANG)
						y = y + dy;*/
					if (level == HERO_LEVEL_DICAUTHANG)
					{
						if ((game->IsKeyDown(DIK_DOWN)))
							if (p->y != 320)
							{
								this->SetPosition(p->x + 3, p->y - 3);
								if (bani == HERO_ANI_ONLYMAN_DOWN1)
									bani = HERO_ANI_ONLYMAN_DOWN2;
								else bani = HERO_ANI_ONLYMAN_DOWN1;

							}
						if ((game->IsKeyDown(DIK_UP)))
						{
							if (bani == HERO_ANI_ONLYMAN_DOWN1)
								bani = HERO_ANI_ONLYMAN_DOWN2;
							else bani = HERO_ANI_ONLYMAN_DOWN1;
							this->SetPosition(p->x + 3, p->y - 33);
						}
					}
				}
				else { if (level == HERO_LEVEL_DICAUTHANG) SetLevel(HERO_LEVEL_ONLYMAN); }

				if (p->type == 13)
				{
					if (p->visible)
						x = x + dx;
				}
				if (level == HERO_LEVEL_ONLYMAN)
					if ((game->IsKeyDown(DIK_DOWN)))
					{
						quaybanhxe = 0;
						this->SetLevel(HERO_LEVEL_BO);
						isjump = false;
					}
				if (level == HERO_LEVEL_BO)
				{
					if ((game->IsKeyDown(DIK_UP)))
					{
						this->SetLevel(HERO_LEVEL_ONLYMAN);
						this->SetPosition(x, y - 10);
						isjump = true;
					}
				}
				else this->setjump(1);
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
	if (state== HERO_STATE_ONLYMANDIE)
	{
		ani = HERO_ANI_ONLYMAN_DIE;
		
	}else
	switch (level)
	{
	
	case HERO_LEVEL_DICAUTHANG:
		if (bani == HERO_ANI_ONLYMAN_DOWN1)
			ani = HERO_ANI_ONLYMAN_DOWN2;
		else
			ani = HERO_ANI_ONLYMAN_DOWN1;
		break;
	case HERO_LEVEL_ONLYMAN:
		if (vx == 0)
		{
			if (nx > 0) ani = HERO_ANI_ONLYMAN_IDLE_LEFT;
			else ani = HERO_ANI_ONLYMAN_IDLE_RIGHT;
		}
		else if (vx > 0)
			ani = HERO_ANI_ONLYMAN_WALKING_LEFT;
		else ani = HERO_ANI_ONLYMAN_WALKING_RIGHT;
		break;
	case HERO_LEVEL_INCAR:
		 if(state>= HERO_STATE_JUMP_UP&& state<= HERO_STATE_UP_WALKING_LEFT)
			if (nx > 0)
				ani = xe_right_up[quaybanhxe ];
			else ani = xe_left_up[quaybanhxe % 4];

		 if (state >= HERO_STATE_IDLE && state <= HERO_STATE_JUMP)
			 if (nx > 0)
				 ani = xe_right[quaybanhxe ];
			 else ani = xe_left[quaybanhxe ];

		 if (state >= HERO_STATE_UP_HALF && state <= HERO_SATE_JUMP_UP_HALF)
			 if (nx > 0)
				 ani = xe_right_up_half[quaybanhxe];
			 else ani = xe_left_up_half[quaybanhxe];
		break;
	case HERO_LEVEL_BO:
		if (nx > 0)
			ani = bo_right[quaybanhxe];	
		else 
			ani = bo_left[quaybanhxe];
		break;
	case HERO_LEVEL_OVH:
		switch (state)
		{
		case HERO_STATE_IDLE:
			if (nx == -1)
				ani = HERO_ANI_OVH_IDLE_LEFT;
			if(nx==1)
				ani = HERO_ANI_OVH_IDLE_RIGHT;
			if (ny == 1)
				ani = HERO_ANI_OVH_IDLE_UNDER; 
			if (ny == -1)
				ani = HERO_ANI_OVH_IDLE_STRAIGHT;
			break;
		case HERO_STATE_GO_STRAIGHT:
			ani = HERO_ANI_OVH_WALKING_STRAIGHT;
			break;
		case HERO_STATE_GO_LEFT:
			ani = HERO_ANI_OVH_WALKING_LEFT;
			break;
		case HERO_STATE_GO_RIGHT:
			ani = HERO_ANI_OVH_WALKING_RIGHT;
			break;
		case HERO_STATE_GO_UNDER:
			ani = HERO_ANI_OVH_WALKING_UNDER;
			break;
		}
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
	case HERO_STATE_IDLE:
		vx = 0;
		if (level == HERO_LEVEL_OVH)
			vy = 0;
		break;
	case HERO_STATE_UP_HALF:
		vx = 0;
		break;
	case HERO_STATE_UP:
		vx = 0;
		break;
	case HERO_STATE_WALKING_RIGHT:
		if (level == HERO_LEVEL_BO) vx = HERO_WALKING_SPEED / 6;
		else;
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_UP_WALKING_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_UP_HALF_WALKING_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		break;
	case HERO_STATE_WALKING_LEFT: 
		if (level == HERO_LEVEL_BO) 
			vx = -HERO_WALKING_SPEED / 6;
		else;
		vx = -HERO_WALKING_SPEED;
		nx = -1;
		break;
	case HERO_STATE_UP_HALF_WALKING_LEFT:
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
	case HERO_SATE_JUMP_UP_HALF:
		vy = -HERO_JUMP_SPEED_Y;
		break;
	case HERO_STATE_JUMP_UP:
		vy = -HERO_JUMP_SPEED_Y;
		break;
	case HERO_STATE_ONLYMANDIE:
		vy = -0;
		break;
	
	case HERO_STATE_GO_STRAIGHT:
		vx = 0;
		vy = -HERO_WALKING_SPEED;
		nx = 0;
		ny = -1;
		break;
	case HERO_STATE_GO_LEFT:
		vx = -HERO_WALKING_SPEED;
		nx = -1;
		ny = 0;
		vy = 0;
		break;
	case HERO_STATE_GO_RIGHT:
		vx = HERO_WALKING_SPEED;
		nx = 1;
		ny = 0;
		vy = 0;
		break;
	case HERO_STATE_GO_UNDER:
		vx = 0;
		vy = HERO_WALKING_SPEED;
		ny = 1;
		nx = 0;
		break;
	}
	//DebugOut(L"[INFO] state: %d\n", state);
	
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
		if (state >= HERO_STATE_JUMP_UP && state <= HERO_STATE_UP_WALKING_LEFT)
		{
			right = x + 26;
			bottom = y + 34;
		}
		if (state >= HERO_STATE_UP_HALF && state <= HERO_SATE_JUMP_UP_HALF)
		{
			right = x + 26;
			bottom = y + 30;
		}
		if (state >= HERO_STATE_IDLE && state <= HERO_STATE_JUMP)
		{
			right = x + HERO_INCAR_BBOX_WIDTH;
			bottom = y + HERO_INCAR_BBOX_HEIGHT;//18
		}
		
		break;
	case HERO_LEVEL_DICAUTHANG:

		right = x + HERO_ONLYMAN_BBOX_WIDTH;
		bottom = y + HERO_ONLYMAN_BBOX_HEIGHT;
		break;
	case HERO_LEVEL_BO:
		right = x + HERO_BO_BBOX_WIDTH;
		bottom = y + HERO_BO_BBOX_HEIGHT;
		break;
	case HERO_LEVEL_OVH:
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
	isup = false;
	SetLevel(start_level);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	quaybanhxe = 0;
	if(level==HERO_LEVEL_INCAR)
		this->car->visible = false;
	if(level==HERO_LEVEL_ONLYMAN)
	car->SetPosition(start_x, start_y);
	this->hp = 100; 
	this->car->y = -1;
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
		quaybanhxe = 3;
		SetPosition(this->car->x, this->car->y+8);
		this->car->y = -1;
		break;
	}
}

