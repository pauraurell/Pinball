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
	iPoint leftLever;
	iPoint rightLever;

	PhysBody* joint1;
	PhysBody* joint2;
	PhysBody* leftLeverBody;
	PhysBody* rightLeverBody;
	b2RevoluteJoint* leftJoint;

};