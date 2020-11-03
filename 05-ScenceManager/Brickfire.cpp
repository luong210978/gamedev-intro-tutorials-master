#include "Brickfire.h"
#include "Sprites.h"
CBrickfire::CBrickfire(float l, float t)
{
	x = l;
	y = t;
}
void CBrickfire::Render()
{
	//animation_set->at(0)->Render(x, y);

}

void CBrickfire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
