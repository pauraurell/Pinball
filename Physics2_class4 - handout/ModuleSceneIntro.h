#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class b2Vec2;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* kicker;

	PhysBody* sensor;
	PhysBody* sensor2;
	PhysBody* sensor3;
	PhysBody* sensor_cahngeSprite;
	PhysBody* sensor_cahngeSprite_out;
	bool sensed;

	int blitTriangles = 0;
	int blitCircles = 0;
	float blitTemp = 0;

	b2Vec2* initialPos = nullptr;

	SDL_Texture*backgroundUpBall;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* tunel;
	SDL_Texture* BrightRound;
	SDL_Texture* BrightTriangular;
	uint bonus_fx;
	uint hit_fx;
	p2Point<int> ray;
	bool ray_on;

	bool tunel_visible = false;
};
