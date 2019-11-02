#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	lever = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	lever = App->textures->Load("pinball/Lever.png");
	leftLever = { 142, 817};
	rightLever = { 267, 817};
	joint1 = App->physics->CreateCircle(leftLever.x+11, leftLever.y+12, 5, b2_staticBody);
	joint2 = App->physics->CreateCircle(rightLever.x+69, rightLever.y+12, 5, b2_staticBody);
	leftLeverBody = App->physics->CreateRectangle(leftLever.x+45, leftLever.y+12, 80, 15, b2_dynamicBody);
	b2RevoluteJointDef def;
	def.Initialize(joint1->body, leftLeverBody->body, joint1->body->GetWorldCenter());
	def.lowerAngle = -0.5f * b2_pi;
	def.upperAngle = 0.25f * b2_pi;
	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	update_status ret = UPDATE_ERROR;
	if (Draw()) {
		ret = UPDATE_CONTINUE;
	}
	return ret;
}


bool ModulePlayer::Draw()
{
	LOG("Drawing levers");

	App->renderer->Blit(lever, leftLever.x, leftLever.y, NULL, 1.0f);
	App->renderer->Blit(lever, rightLever.x, rightLever.y, NULL, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);

	return true;
}