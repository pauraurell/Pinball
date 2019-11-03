#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"

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
	kickerBackground = App->textures->Load("pinball/kicker_sprite.png");
	kicker = App->textures->Load("pinball/kicker.png");
	leftLever = { 142, 817};
	rightLever = { 267, 817};
	kickerPos = { 490, 780 };
	joint1 = App->physics->CreateCircle(leftLever.x+11, leftLever.y+12, 5, b2_staticBody);
	joint2 = App->physics->CreateCircle(rightLever.x+69, rightLever.y+12, 5, b2_staticBody);
	joint3 = App->physics->CreateCircle(kickerPos.x + 9, kickerPos.y + 12, 5, b2_staticBody);
	
	int Lever[16] = {
		12, 3,
		20, 5,
		84, 43,
		84, 47,
		78, 50,
		8, 24,
		4, 16,
		7, 7
	};
	int Lever2[16] = {
		-12, 3,
		-20, 5,
		-84, 43,
		-84, 47,
		-78, 50,
		-8, 24,
		-4, 16,
		-7, 7
	};

	int size = 16;

	leftLeverBody = App->physics->CreatePolygon(leftLever.x, leftLever.y, Lever, size, b2_dynamicBody, 1.0f, 10.0f);
	rightLeverBody = App->physics->CreatePolygon(rightLever.x+80, rightLever.y, Lever2, size, b2_dynamicBody, 1.0f, 10.0f);
	kickerBody = App->physics->CreateRectangle(kickerPos.x, kickerPos.y, 25, 5, b2_dynamicBody, 0);
	
	//Joint left lever
	b2RevoluteJointDef def;
	def.Initialize(joint1->body, leftLeverBody->body, joint1->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = -0.35f * b2_pi;
	def.upperAngle = 0;
	def.maxMotorTorque = 150;
	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);
	//Joint right lever
	def.Initialize(joint2->body, rightLeverBody->body, joint2->body->GetWorldCenter());
	def.enableLimit = true;
	def.lowerAngle = 0;
	def.upperAngle = 0.35f * b2_pi;
	def.maxMotorTorque =150;
	rightJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def);

	b2PrismaticJointDef def2;
	//def2.Initialize(kickerJoint->body, kickerBody->body, kickerJoint->body->GetWorldCenter(), { 0, 1 });
	def2.bodyA = joint3->body;
	def2.bodyB = kickerBody->body;
	def2.localAnchorA = joint3->body->GetWorldCenter();
	def2.localAnchorB = kickerBody->body->GetWorldCenter();
	def2.localAxisA = { 0, 1 };
	def2.collideConnected = false;
	/*def2.enableLimit = true;
	def2.lowerTranslation = PIXEL_TO_METERS(499);
	def2.upperTranslation = PIXEL_TO_METERS(471);*/
	//def2.maxMotorForce = 150;
	kickerJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&def2);

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		leftJoint->EnableMotor(true);
		leftJoint->SetMotorSpeed(-50);
	}
	else {
		leftJoint->SetMotorSpeed(0);
		leftJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		rightJoint->EnableMotor(true);
		rightJoint->SetMotorSpeed(50);
	}
	else {
		rightJoint->SetMotorSpeed(0);
		rightJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (kickerPos.y < 845)
		{
			kickerPos.y += 2;
			
			if (kickerCounter < 56)
			{
				kickerCounter += 2;
			}
		}
	}

	else  {
		if (kickerPos.y > 780)
		{
			kickerPos.y -= 15;
		}
		if (kickerPos.y < 780)
		{
			kickerPos.y = 780;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		if (App->scene_intro->circles.getFirst() != NULL)
		{
			//kickerBody->body->ApplyLinearImpulse({ 0, 60 }, kickerBody->body->GetWorldCenter(), true);
			App->scene_intro->circles.getFirst()->data->body->ApplyLinearImpulse({ 0, kickerCounter / 8 }, App->scene_intro->circles.getFirst()->data->body->GetWorldCenter(), true);
			kickerCounter = 0;
		}
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
	App->renderer->Blit(kicker, kickerPos.x, kickerPos.y, NULL, 1.0f, 0.0f, 10, 10);
	App->renderer->Blit(kickerBackground,475, 780, NULL);

	return true;
}