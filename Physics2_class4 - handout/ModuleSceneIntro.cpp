#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "p2SString.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	hit_fx = App->audio->LoadFx("pinball/hit.ogg");
	hit_circle_fx = App->audio->LoadFx("pinball/Track2.wav");
	notCool_fx = App->audio->LoadFx("pinball/Track3.wav");
	background = App->textures->Load("pinball/Pinball_Sritesheet.png");
	backgroundUpBall = App->textures->Load("pinball/ElementsUpTheBall.png");
	tunel = App->textures->Load("pinball/Pinball_Tunel.png");
	BrightRound = App->textures->Load("pinball/Round_coll.png");
	BrightTriangular = App->textures->Load("pinball/Bright_Triangular_collider.png");
	tube = App->textures->Load("pinball/tube.png");
	lights = App->textures->Load("pinball/Pinball_SritesheetLights.png");
	fontID = App->fonts->Load("pinball/Fonts.png", "0123456789", 1, 22, 33, 10);

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 20, SCREEN_WIDTH, 50);
	sensor2 = App->physics->CreateRectangleSensor(155, 232, 28, 28);
	sensor3 = App->physics->CreateRectangleSensor(325, 354, 28, 28);
	sensor_changeSprite = App->physics->CreateRectangleSensor(94, 328, 40, 28);
	sensor_changeSprite_out = App->physics->CreateRectangleSensor(76, 730, 40, 28);
	sensor_changeSprite2 = App->physics->CreateRectangleSensor(385, 360, 40, 28);
	sensor_changeSprite2_out = App->physics->CreateRectangleSensor(410, 750, 40, 28);
	sensorLights = App->physics->CreateRectangleSensor(320, 90, 120, 22);
	sensorSkill = App->physics->CreateRectangleSensor(458, 600, 30, 22);

	int size = 8;
	door = App->physics->CreateChain(1, 1, Pinball_door, size, b2_staticBody);

	initialPos = new b2Vec2(10, 15);
	outPos = new b2Vec2(300, 0);
	teleportPos = new b2Vec2(1.5f, 12.5f);

	tunel1force = new b2Vec2(-70, -90);
	tunel2force = new b2Vec2(30, -100);


	circles.add(App->physics->CreateCircle(METERS_TO_PIXELS(initialPos->x), METERS_TO_PIXELS(initialPos->y) + 10, 12, b2_dynamicBody, 0.4f));
	circles.getLast()->data->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	p2SString title("Pinball");
	
	App->window->SetTitle(title.GetString());
	App->renderer->Blit(background, 0, 0, NULL);
	if(tunel_visible)App->renderer->Blit(tunel, 0, 0, NULL);
	if (Lights == true) { App->renderer->Blit(lights, 0, 0, NULL); }
	 if (tunel_visible) {
		 App->renderer->Blit(tunel, 0, 0, NULL);
	 }

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	/*if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12, b2_dynamicBody, 0.4f));
		circles.getLast()->data->listener = this;
	}*/

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
	 	if (tunel_visible == false){tunel_visible = true;}
		else if (tunel_visible == true) { tunel_visible = false; }
	}

	if (resetPos) {
		if (circles.getFirst() != NULL)
		{
			if (respawnTimer == false)
			{
				respawn_timer = SDL_GetTicks();
				respawnTimer = true;
			}

			if (SDL_GetTicks() - respawn_timer > 2800) {
				respawnTimer = false;
				circles.getFirst()->data->body->SetLinearVelocity(b2Vec2_zero);
				circles.getFirst()->data->body->SetTransform(*initialPos, 0);
				resetPos = false;
			}
			Lights = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (circles.getFirst() != NULL)
		{
			circles.getFirst()->data->body->SetLinearVelocity(b2Vec2_zero);
			circles.getFirst()->data->body->SetTransform(*initialPos, 0);
			resetPos = false;
			Lights = false;
		}
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		
		App->renderer->Blit(circle, x, y, NULL, 1.0f);
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	
	if (tunel_visible) {
		p2List_item<PhysBody*>* w = App->physics->walls.getFirst();
		while (w != NULL)
		{
			w->data->body->SetTransform(*outPos, 0);
			w = w->next;
		}
		PhysBody* data_;
		App->physics->circles.at(2, data_);
		data_->body->SetTransform(*outPos, 0);
		App->physics->tunelCol->body->SetTransform(b2Vec2_zero, 0);
	}
	else if (tunel_2_enabled) {
		p2List_item<PhysBody*>* w = App->physics->walls.getFirst();
		while (w != NULL)
		{
			w->data->body->SetTransform(*outPos, 0);
			w = w->next;
		}
		PhysBody* data_;
		App->physics->circles.at(2, data_);
		data_->body->SetTransform(*outPos, 0);
		App->physics->tunel2Col_interior->body->SetTransform(b2Vec2_zero, 0);
		App->physics->tunel2Col->body->SetTransform(b2Vec2_zero, 0);
	}
	else {
		p2List_item<PhysBody*>* w = App->physics->walls.getFirst();
		
		while (w != NULL)
		{
			w->data->body->SetTransform(b2Vec2_zero, 0);
			w = w->next;
		}
		PhysBody* data_;
		b2Vec2* temp = new b2Vec2(PIXEL_TO_METERS(App->physics->ball3pos.x), PIXEL_TO_METERS(App->physics->ball3pos.y));
		App->physics->circles.at(2, data_);
		data_->body->SetTransform(*temp, 0);

		App->physics->tunelCol->body->SetTransform(*outPos, 0);
		App->physics->tunel2Col_interior->body->SetTransform(*outPos, 0);
		App->physics->tunel2Col->body->SetTransform(*outPos, 0);
	}
	if (tele) {
		circles.getFirst()->data->body->SetTransform(*teleportPos, 0);
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2_zero);
		tele = false;
	}
	if (doorOpen > 1) {
		door->body->SetTransform(*initialPos, 0);
		if (doorOpen <= 1.5f) {
			door->body->SetTransform(b2Vec2_zero, 0);
			doorOpen = 0;
		}
		else {
			doorOpen -= 0.01f;
		}
	}

	if (sensorStop > 1) {
		if (sensorStop <= 1.5f) {
			App->physics->NoGravity(circles.getFirst()->data->body);
			sensorStop = 0;
		}
		else {
			sensorStop -= 0.01f;
		}
	}

	if (blitTriangles == 1) {
		if (blitTemp <= 0) {
			blitTriangles = 0;
		}
		else {
			App->renderer->Blit(BrightTriangular, 85, 680, NULL, 1.0f);
			blitTemp -= 0.1f;
		}
	}
	if (blitTriangles == 2) {
		if (blitTemp <= 0) {
			blitTriangles = 0;
		}
		else {
			App->renderer->Blit(BrightTriangular, 340, 680, NULL, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
			blitTemp -= 0.1f;
		}
	}
	if (blitCircles == 1) {
		if (blitTemp <= 0) {
			blitCircles = 0;
		}
		else {
			App->renderer->Blit(BrightRound, 251, 153, NULL, 1.0f);
			blitTemp -= 0.1f;
		}
	}
	if (blitCircles == 2) {
		if (blitTemp <= 0) {
			blitCircles = 0;
		}
		else {
			App->renderer->Blit(BrightRound, 361, 148, NULL, 1.0f);
			blitTemp -= 0.1f;
		}
	}
	if (blitCircles == 3) {
		if (blitTemp <= 0) {
			blitCircles = 0;
		}
		else {
			App->renderer->Blit(BrightRound, 310, 210, NULL, 1.0f);
			blitTemp -= 0.1f;
		}
	}
	if (tunel_2_enabled == false) { App->renderer->Blit(backgroundUpBall, 0, 0, NULL); }
	if (tunel_2_enabled == true) { App->renderer->Blit(tube, 0, 0, NULL); }
	

	SDL_snprintf(pointsText, 10, "%d", points);
	App->fonts->BlitText(600, 400, fontID, pointsText);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(hit_fx);
	bool collided = false;
	
	p2List_item<PhysBody*>* c = circles.getFirst();
	p2List_item<PhysBody*>* t = App->physics->triangles.getFirst();
	p2List_item<PhysBody*>* c2 = App->physics->circles.getFirst();
	p2List_item<PhysBody*>* s = App->physics->sensors.getFirst();
	p2List_item<PhysBody*>* w = App->physics->walls.getFirst();

	while (c != NULL)
	{
		while (t != NULL && collided == false)
		{
			if (bodyA == c->data && bodyB == t->data)
			{
				points += trianglePoints;
				blitTemp = 1.0f;
				if (c->data->body->GetTransform().p.x < 4) blitTriangles = 1;
				else blitTriangles = 2;
				collided = true;
				LOG("Points: %i", points);
			}
			t = t->next;
		}
		while (c2 != NULL && collided == false)
		{
			if (bodyA == c->data && bodyB == c2->data)
			{
				App->audio->PlayFx(hit_circle_fx);
				points += circlePoints;
				blitTemp = 1.0f;
				if (c2->data == App->physics->circles.getFirst()->data) { blitCircles = 1; }
				else if (c->data->body->GetTransform().p.x > 7 && c->data->body->GetTransform().p.y < 4.22f) { blitCircles = 2; }
				else { blitCircles = 3; }
				collided = true;
				LOG("Points: %i", points);
			}
			c2 = c2->next;
		}
		while (s != NULL && collided == false)
		{
			if (bodyA == c->data && bodyB == s->data)
			{
				blitTemp = 1.0f;
				if (s->data == sensor_changeSprite) { 
					tunel_visible = true;
					if(sensorStop == 0)	App->physics->NoGravity(c->data->body);
					c->data->body->ApplyForceToCenter(*tunel1force, false);
					sensorStop = 3;
					points += tunelPoints;
				}
				if (s->data == sensor_changeSprite_out) tunel_visible = false; 
				if (s->data == sensor_changeSprite2) { 
					tunel_2_enabled = true;
					if (sensorStop == 0) App->physics->NoGravity(c->data->body);
					c->data->body->ApplyForceToCenter(*tunel2force, false);
					sensorStop = 3;
					points += tunelPoints;
				}
				if (s->data == sensorLights) {
					points += LightsPoints;
					Lights = true;
				}
				if (s->data == sensor_changeSprite2_out) tunel_2_enabled = false;
				if (s->data == App->player->kickerSensor) kickerActive = true;
				else kickerActive = false;
				collided = true;
				LOG("Points: %i", points);

				if (s->data == sensor3 && sensorStop == 0) 
				{
					points += gravityPoints;
					App->physics->NoGravity(c->data->body);
					sensorStop = 3;
				}
				else if (s->data == sensor2) {
					points += teleporterPoints;
					tele = true;
				}
				else if (s->data == sensor && kickerActive == false) {
					App->audio->PlayFx(notCool_fx);
					resetPos = true;
					App->audio->PlayFx(notCool_fx);
				}

				if (s->data == App->player->doorOpenCol) doorOpen = 2.0f;
			}
			s = s->next;
		}

		collided = false;
		c = c->next;
	}
}
