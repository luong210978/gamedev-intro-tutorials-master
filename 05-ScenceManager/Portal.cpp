#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id ,int p,int lv)
{
	this->scene_id = scene_id;
	this->scene_place=p;
	this->scene_level = lv;
	x = l; 
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
