#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Brickfire.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "HERO.h"
class CPlayScene: public CScene
{
protected: 
	CHERO *player;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSeciton_MAP(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load(int crp, int lv);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CHERO * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
protected:
	vector<LPGAMEOBJECT> objects2;
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

