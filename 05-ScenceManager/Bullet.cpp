#include "Bullet.h"
#include "Goomba.h"
#define MAX_FRAME_RATE 120

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;	
	bottom = y + BULLET_BBOX_HEIGHT;
}
CBullet::CBullet(float l, float t, int k) {
	this->x = l; this->y = t; this -> nx = k;
	startPositionX = x;
	startPositionY = y;
};

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (nx < 0) {
		SetSpeed(-BULLET_SPEED, 0.0f);
		if (x < startPositionX - BULLET_FLYING_SPACE)
		{
			x = NULL;
			y = NULL;
			return;			
		}
		else {
			x += dx;
			y += dy;
		}

	}
	else {
		SetSpeed(BULLET_SPEED, 0.0f);
		if (x > startPositionX + BULLET_FLYING_SPACE) {
			x = NULL;
			y = NULL;
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
		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

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
		this->x = NULL;
		this->y = NULL;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CBullet::Render()
{
	int ani=1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(22);
	this->SetAnimationSet(ani_set);
	if (nx == -1) {
		ani = BULLET_STATE_FLYING_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		ani = BULLET_STATE_FLYING_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
}
void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	return;
}
