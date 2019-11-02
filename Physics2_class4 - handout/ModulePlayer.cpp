#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
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
	/*
	int Lever[20] = {
	8, 3,
	15, 3,
	25, 8,
	79, 40,
	79, 45,
	75, 46,
	58, 41,
	6, 22,
	2, 16,
	3, 8
	};

	leftLeverBody = App->physics->CreateChain(leftLever.x, leftLever.y, Lever, 20, b2_dynamicBody);
	*/
	leftLeverBody = App->physics->CreateRectangle(leftLever.x+50, leftLever.y+12, 80, 15, b2_dynamicBody);
	rightLeverBody = App->physics->CreateRectangle(leftLever.x+155, leftLever.y+12, 80, 15, b2_dynamicBody);
	//Joint left lever
	b2RevoluteJointDef def;
	def.Initialize(joint1->body, leftLeverBody->body, joint1->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = -0.25f * b2_pi;
	def.upperAngle = 0.15f * b2_pi;
	def.maxMotorTorque = 80;
	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);
	//Joint right lever
	def.Initialize(joint2->body, rightLeverBody->body, joint2->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = -0.15f * b2_pi;
	def.upperAngle = 0.25f * b2_pi;
	def.maxMotorTorque = 80;
	rightJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);

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

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) {
		leftJoint->EnableMotor(true);
		leftJoint->SetMotorSpeed(-50);
	}
	else {
		leftJoint->SetMotorSpeed(0);
		leftJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		rightJoint->EnableMotor(true);
		rightJoint->SetMotorSpeed(50);
	}
	else {
		rightJoint->SetMotorSpeed(0);
		rightJoint->EnableMotor(false);
	}


	if (Draw()) {
		ret = UPDATE_CONTINUE;
	}

	return ret;
}


bool ModulePlayer::Draw()
{
	LOG("Drawing levers");

	App->renderer->Blit(lever, leftLever.x, leftLever.y, NULL, 1.0f, leftLeverBody->GetRotation()-24.0f, 10, 10);
	App->renderer->Blit(lever, rightLever.x, rightLever.y, NULL, 1.0f, rightLeverBody->GetRotation()+24.0f, 70, 10, SDL_FLIP_HORIZONTAL);

	return true;
}