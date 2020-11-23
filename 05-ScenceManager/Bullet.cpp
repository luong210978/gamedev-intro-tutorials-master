#include "Bullet.h"
#include "Goomba.h"
#include "Brick.h"
#define MAX_FRAME_RATE 120

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (aniid == 22)
	{
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	}
	if (aniid == 23)
	{
		right = x + BULLET_BBOX_WIDTH/2;
		bottom = y + BULLET_BBOX_HEIGHT/2;
	}
	if (aniid == 24)
	{
		right = x + 22;
		bottom = y + 6;
	}
	if (aniid==25)
	{
		right = x + 6;
		bottom = y + 22;
	}
}
CBullet::CBullet(float l, float t, int k,int aniid) {
	this->x = l; this->y = t; this -> nx = k;
	startPositionX = x;
	startPositionY = y;
	this->del = false;
	this->aniid = aniid;
};

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if(aniid==25)
	{
		SetSpeed(0.0f, - BULLET_SPEED );
		if (y < startPositionY - BULLET_FLYING_SPACE)
		{
			del = true;
			return;
		}
		else {
			x += dx;
			y += dy;
		}
	}
	else
	if (nx == -1) {
		SetSpeed(-BULLET_SPEED, 0.0f);
		if (x < startPositionX - BULLET_FLYING_SPACE)
		{
			del = true;
			return;			
		}
		else {
			x += dx;
			y += dy;
		}

	}
	else 
	if (nx == 1)
	{
		SetSpeed(BULLET_SPEED, 0.0f);
		if (x > startPositionX + BULLET_FLYING_SPACE) {
			
			del = true; 
			return;
			
		}
		else
		{
			x += dx;
			y += dy;
		}
	}
vector<LPCOLLISIONEVENT> coEvents;
vector<LPCOLLISIONEVENT> coEventsResult;
CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny=0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny,rdx,rdy);
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		del = true;
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT e = coEvents[i];
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* p = dynamic_cast<CBrick*>(e->obj);
				if (p->type == 13)
				{
					p->del = true;
				}
			}
		}
		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->nx != 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->visible = false;
					}
				}				
			}
			
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CBullet::Render()
{
	int ani=1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(aniid);
	this->SetAnimationSet(ani_set);
	if (nx == -1) 
	{
		ani = BULLET_STATE_FLYING_LEFT;
		animation_set->at(ani)->Render(x, y,255);
	}
	else
	{
		ani = BULLET_STATE_FLYING_RIGHT;
		animation_set->at(ani)->Render(x, y);
	}
}
void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	return;
}
