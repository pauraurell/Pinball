#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "Box2D/Box2D/Box2D.h"


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	bool Draw();

public:

	SDL_Texture* lever;
	SDL_Texture* kicker;
	SDL_Texture* kickerBackground;

	iPoint leftLever;
	iPoint rightLever;
	b2Vec2 kickerPos;
	float kickerCounter = 0;

	PhysBody* joint1;
	PhysBody* joint2;
	PhysBody* joint3;
	PhysBody* leftLeverBody;
	PhysBody* rightLeverBody;
	b2RevoluteJoint* leftJoint;
	b2RevoluteJoint* rightJoint;
	b2PrismaticJoint* kickerJoint;

	PhysBody* kickerBody;
	PhysBody* kickerSensor;
	PhysBody* doorOpenCol;

	
};