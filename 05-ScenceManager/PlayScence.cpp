#include <iostream>
#include <fstream>
#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Bullet.h"
#include "Map.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id,filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP 7
#define OBJECT_TYPE_HERO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_BRICKFIRE	11
#define OBJECT_TYPE_thang	12
#define OBJECT_TYPE_BULLET	100

#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50
#define MAX_SCENE_LINE 1024

Map* map=NULL;
int current_place;
int current_level;
void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str())*16;
	float y = atof(tokens[2].c_str())*16;

	int ani_set_id = atoi(tokens[3].c_str());
	
	CGameObject* obj = NULL;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	switch (object_type)
	{
	case OBJECT_TYPE_HERO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] HERO object was created before!\n");
			return;
		}
		if (atof(tokens[4].c_str()) == current_place)
		{
			obj = new CHERO(x, y);
			player = (CHERO*)obj;
			player->car = new Ccar(x,y,1);			
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			player->car->SetAnimationSet(ani_set);
			objects.push_back(player->car);
		}
		else
		{
			return;
		}

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK:
	{
		obj = new CBrick(x, y, object_type);
	}
	break;
	case OBJECT_TYPE_BRICKFIRE:
	{
		obj = new CBrick(x, y, object_type);
	}
	case OBJECT_TYPE_thang:
	{
		obj = new CBrick(x, y, object_type);
	}

	break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas();
		break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());

		int scene_id = atoi(tokens[6].c_str());
		int scene_place = atoi(tokens[7].c_str());
		int scene_level = atoi(tokens[8].c_str());
		obj = new CPortal(x, y, r * 16, b * 16, scene_id, scene_place,scene_level);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}
void CPlayScene::_ParseSeciton_MAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 6) return;
	float x = atof(tokens[0].c_str());
	float y = atof(tokens[1].c_str());
	float z = atof(tokens[2].c_str());
	float t = atof(tokens[3].c_str());
	float k = atof(tokens[4].c_str());
	float j = atof(tokens[5].c_str());
	wstring mt = ToWSTR(tokens[6].c_str());
	map = new Map(x, y, z, t, k, j);
	map->LoadMatrix(mt.c_str());
	map->CreateTilesFromTileSet();
}
void CPlayScene::Load(int crp,int lv)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	current_place = crp;
	current_level = lv;
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP]")
		{
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	
		
		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP:_ParseSeciton_MAP(line); break;
		}
		
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	player->SetLevel(current_level);
	player->hp = 100;
}

void CPlayScene::Update(DWORD dt)
{
	// We know that HERO is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (HERO::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow HERO
	float cx, cy;
	player->GetPosition(cx, cy);
	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -=game->GetScreenHeight() / 2;
	
	CGame::GetInstance()->SetCamPos(round(cx), round(cy));
}
CGameObject* obj = NULL;
void CPlayScene::Render()
{
	float cx, cy;
	player->GetPosition(cx, cy);
	map->Render(round(cx),round(cy));
	if (player->car->visible == true)
		objects[0]->Render();
	if (player->isban == true)
	{
		if (player->GetLevel() == HERO_LEVEL_ONLYMAN)
		{
			if (player->nx == -1)
			{
				player->isban = false;
				obj = new CBullet(cx - BULLET_BBOX_WIDTH, cy + HERO_ONLYMAN_BBOX_HEIGHT / 4, -1, 22);
				objects.push_back(obj);
			}
			else
			{
				player->isban = false;
				CGameObject* obj = NULL;
				obj = new CBullet(cx + HERO_ONLYMAN_BBOX_WIDTH, cy + HERO_ONLYMAN_BBOX_HEIGHT / 4, 1, 22);
				objects.push_back(obj);
			}
		}
		if (player->GetLevel() == HERO_LEVEl_BO)
		{
			if (player->nx == -1)
			{
				player->isban = false;
				obj = new CBullet(cx - BULLET_BBOX_WIDTH/2, cy + HERO_BO_BBOX_HEIGHT / 2, -1, 23);
				objects.push_back(obj);
			}
			else
			{
				player->isban = false;
				CGameObject* obj = NULL;
				obj = new CBullet(cx + HERO_BO_BBOX_WIDTH, cy + HERO_BO_BBOX_HEIGHT / 2, 1, 23);
				objects.push_back(obj);
			}
		}
		if (player->GetLevel() == HERO_LEVEL_INCAR)
		{
			if(player->state>= 600)
			{
				player->isban = false;
				obj = new CBullet(cx + 10, cy -6, 1, 25);
				objects.push_back(obj);
			}
			else
			if (player->nx == -1)
			{
				player->isban = false;
				obj = new CBullet(cx - 22, cy + HERO_INCAR_BBOX_HEIGHT / 5, -1, 24);
				objects.push_back(obj);
			}
			else
			{
				player->isban = false;
				CGameObject* obj = NULL;
				obj = new CBullet(cx + HERO_INCAR_BBOX_WIDTH, cy + HERO_INCAR_BBOX_HEIGHT / 5, 1, 24);
				objects.push_back(obj);
			}
		}
		
	}
	for (int i = 1; i < objects.size(); i++)
		if (objects[i]->x > NULL+20)
			objects[i]->Render();
		
	
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	
	CHERO *HERO = ((CPlayScene*)scence)->GetPlayer();
	if (KeyCode == DIK_C)
		//if (HERO->GetLevel() == HERO_LEVEL_ONLYMAN)
			HERO->isban = true;
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (HERO->getjump())
			if(HERO->state==HERO_STATE_UP)
			HERO->SetState(HERO_STATE_JUMP_UP);
			else 			HERO->SetState(HERO_STATE_JUMP);

			
		HERO->setjump(0);
		break;
	
	case DIK_A:
		HERO->Reset();
		break;
	case DIK_X:
		if (HERO->getjump())
		if (HERO->GetLevel() == HERO_LEVEL_INCAR)
		{
			HERO->car->visible = true;
			HERO->car->SetPosition(HERO->x, HERO->y);
			HERO->car->Setn(HERO->nx);
			HERO->Changelevel();

		}
		else 
			if (HERO->GetLevel()== HERO_LEVEL_ONLYMAN)
				if (HERO->getinto == true)
				{
					HERO->nx = HERO->car->nx;
					HERO->Changelevel();
					HERO->car->visible = false;

				}
		break;
	}
	
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CHERO *HERO = ((CPlayScene*)scence)->GetPlayer();
	
	// disable control key when HERO ONLYMANDIE 
	if (HERO->GetState() == HERO_STATE_ONLYMANDIE) 
		return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (HERO->GetLevel() == HERO_LEVEL_INCAR)
			if (game->IsKeyDown(DIK_UP))
			{
				if (HERO->state == HERO_STATE_WALKING_RIGHT)
					HERO->SetPosition(HERO->x, HERO->y - 16);
				HERO->SetState(HERO_STATE_UP_WALKING_RIGHT);
				return;
			}			
		HERO->SetState(HERO_STATE_WALKING_RIGHT);
		if (HERO->bani == HERO_ANI_BO_RIGHT1)
			HERO->bani = HERO_ANI_BO_RIGHT2;
			else 
			HERO->bani = HERO_ANI_BO_RIGHT1;
		
	}	
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (HERO->GetLevel() == HERO_LEVEL_INCAR)
			if (game->IsKeyDown(DIK_UP))
			{
				if (HERO->state == HERO_STATE_WALKING_LEFT)
					HERO->SetPosition(HERO->x, HERO->y - 16);
				HERO->SetState(HERO_STATE_UP_WALKING_LEFT);	
				return;
			}			
		HERO->SetState(HERO_STATE_WALKING_LEFT);
		if (HERO->bani == HERO_ANI_BO_LEFT1)
			HERO->bani = HERO_ANI_BO_LEFT2;
			else 
			HERO->bani = HERO_ANI_BO_LEFT1;
	
		//HERO->setjump(0);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (HERO->GetLevel() == HERO_LEVEL_INCAR)
		{
			if (HERO->state == HERO_STATE_IDLE)
				HERO->SetPosition(HERO->x, HERO->y - 16);
			HERO->SetState(HERO_STATE_UP);
		}
	}
	else
	{
		HERO->SetState(HERO_STATE_IDLE);
	}
}