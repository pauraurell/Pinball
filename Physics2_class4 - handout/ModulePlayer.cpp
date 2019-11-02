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
	
	int Lever[16] = {
			11, 4,
			3, 17,
			7, 21,
			73, 45,
			78, 45,
			79, 42,
			77, 39,
			22, 7
	};
	int Lever2[16] = {
		-11, 4,
		-3, 17,
		-7, 21,
		-73, 45,
		-78, 45,
		-79, 42,
		-77, 39,
		-22, 7
	};

	int size = 16;

	leftLeverBody = App->physics->CreatePolygon(leftLever.x, leftLever.y, Lever, size, b2_dynamicBody);
	rightLeverBody = App->physics->CreatePolygon(rightLever.x+80, rightLever.y, Lever2, size, b2_dynamicBody);
	
	//Joint left lever
	b2RevoluteJointDef def;
	def.Initialize(joint1->body, leftLeverBody->body, joint1->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = -0.35f * b2_pi;
	def.upperAngle = 0;
	def.maxMotorTorque = 80;
	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);
	//Joint right lever
	def.Initialize(joint2->body, rightLeverBody->body, joint2->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = 0;
	def.upperAngle = 0.35f * b2_pi;
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
	App->renderer->Blit(lever, leftLever.x, leftLever.y, NULL, 1.0f, leftLeverBody->GetRotation(), 10, 10);
	App->renderer->Blit(lever, rightLever.x, rightLever.y, NULL, 1.0f, rightLeverBody->GetRotation(), 70, 10, SDL_FLIP_HORIZONTAL);

	return true;
}