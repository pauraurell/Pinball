#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

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

	return UPDATE_CONTINUE;
}


bool ModulePlayer::Draw()
{
	LOG("Drawing levers");

	App->renderer->Blit(lever, leftLever.x, leftLever.y, NULL, 1.0f, leftRotation);
	App->renderer->Blit(lever, rightLever.x, rightLever.y, NULL, 1.0f, rightRotation, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);

	return true;
}