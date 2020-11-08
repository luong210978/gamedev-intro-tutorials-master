#include "car.h"
#include "Goomba.h"
#define MAX_FRAME_RATE 120

void Ccar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CAR_BBOX_WIDTH;
	bottom = y + CAR_BBOX_HEIGHT;
}
Ccar::Ccar(float l, float t, int k) {
	this->x = l; this->y = t; this->nx = k;
	startPositionX = x;
	startPositionY = y;
};

void Ccar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	SetSpeed(0.0f,CAR_SPEED);	
	x += dx;
	y += dy;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		this->SetSpeed(0.0f, 0.0f);

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;			
		this->SetSpeed(0.0f, 0.0f);

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
void Ccar::Render()
{
	int ani = 1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(1);
	this->SetAnimationSet(ani_set);
	if (nx == -1) {
		ani = ani = CAR_STATE_FLYING_RIGHT;
		
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		ani = CAR_STATE_FLYING_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
}
void Ccar::Setn(int n)
{
	nx = n;
	return;
}
