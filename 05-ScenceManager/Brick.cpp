#include "Brick.h"
#include "Sprites.h"
CBrick::CBrick(float l, float t,int k,int r,int b)
{
	x = l;
	y = t;
	type = k;
	this->r = r;
	this->b = b;
}
void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	
}

float CBrick::Getlosehp(int type)
{
	
	
	if (type == 11)
		return IGNITE;
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = this->r;
	b = this->b;
}
