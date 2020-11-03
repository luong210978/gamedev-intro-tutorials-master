#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrickfire : public CGameObject
{
public:
	CBrickfire(float l, float t);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
#pragma once

