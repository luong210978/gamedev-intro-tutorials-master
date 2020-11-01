#include "Brick.h"
#include "Sprites.h"
CBrick::CBrick(float l, float t)
{
	x = l;
	y = t;
}
void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
