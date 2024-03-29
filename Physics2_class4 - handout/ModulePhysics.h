#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"


#define GRAVITY_Y -15.5f
#define GRAVITY_X 0.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type, float restitution = 0.0f);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type, float density = 1.0f);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType type, float restitution = 0.0f, float gravity = 1.0f);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type, float restitution = 0.0f);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	p2List<PhysBody*> bouncyness;
	p2List<PhysBody*> walls;
	p2List<PhysBody*> triangles;
	p2List<PhysBody*> circles;
	p2List<PhysBody*> sensors;

	void NoGravity(b2Body *body);

	b2World* world;
	PhysBody* tunelCol = nullptr;
	PhysBody* tunel2Col = nullptr;
	PhysBody*tunel2Col_interior = nullptr;

	iPoint ball3pos;

private:

	bool debug = true;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};