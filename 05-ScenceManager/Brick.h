#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define IGNITE 3.f
class CBrick : public CGameObject
{	
public:
	CBrick(float l, float t,int k);
	virtual void Render();
	virtual float Getlosehp(int type);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
#pragma once

