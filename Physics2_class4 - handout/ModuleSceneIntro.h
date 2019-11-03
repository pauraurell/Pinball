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

	PhysBody* door;
	int Pinball_door[8] = {
		477, 452,
		500, 423,
		505, 428,
		483, 455
	};

	PhysBody* sensor;
	PhysBody* sensor2;
	PhysBody* sensor3;
	PhysBody* sensor_changeSprite;
	PhysBody* sensor_changeSprite_out;
	PhysBody* sensor_changeSprite2;
	PhysBody* sensor_changeSprite2_out;
	bool sensed;
	bool kickerActive = false;
	bool resetPos = false;
	float doorOpen = 0;
	float sensorStop = 0;

	int points = 0;
	int circlePoints = 20;
	int tunelPoints = 50;
	int trianglePoints = 10;
	int gravityPoints = 20;


	int blitTriangles = 0;
	int blitCircles = 0;
	float blitTemp = 0;

	b2Vec2* initialPos = nullptr;
	b2Vec2* outPos = nullptr;

	b2Vec2* tunel1force = nullptr;
	b2Vec2* tunel2force = nullptr;

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
	uint hit_circle_fx;
	uint notCool_fx;

	p2Point<int> ray;
	bool ray_on;

	bool tunel_visible = false;
	bool tunel_2_enabled = false;
};
