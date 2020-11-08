#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	int scene_place;
	int scene_level;
	int width;
	int height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id,int p,int lv);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
	int GetScenePlace() { return scene_place; }
	int GetSceneLevel() { return scene_level; }
};