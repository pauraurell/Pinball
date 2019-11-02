#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/DebugBox2D/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/ReleaseBox2D/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// outline collider of the pinball scene
	int x = 1;
	int y = 1;
	
	int pinball[62] = {
		270, 8,
		339, 16,
		397, 38,
		451, 80,
		486, 136,
		504, 203,
		510, 268,
		510, 346,
		510, 509,
		510, 785,
		471, 785,
		471, 804,
		434, 823,
		327, 875,
		327, 927,
		160, 927,
		160, 874,
		52, 822,
		20, 807,
		19, 661,
		25, 648,
		75, 608,
		76, 515,
		48, 440,
		32, 363,
		24, 284,
		31, 208,
		53, 141,
		88, 88,
		144, 41,
		203, 18
	};
	int size = 62;
	CreateChain(x, y, pinball, size, b2_staticBody);
	
	CreateCircle(277, 178, 23, b2_staticBody);
	CreateCircle(335, 236, 23, b2_staticBody);

	int triangleLeft[18] = {
		93, 759,
		89, 746,
		89, 689,
		94, 685,
		101, 686,
		150, 774,
		150, 778,
		145, 782,
		140, 782
	};
	size = 18;
	CreateChain(x, y, triangleLeft, size, b2_staticBody);

	int triangleRight[20] = {
		401, 759,
		405, 749,
		404, 693,
		401, 686,
		394, 686,
		390, 690,
		345, 770,
		345, 776,
		349, 780,
		356, 780
	};
	size = 20;
	CreateChain(x, y, triangleRight, size, b2_staticBody);

	int leftWall[20] = {
		48, 686,
		54, 686,
		54, 770,
		57, 774,
		147, 815,
		147, 821,
		143, 824,
		50, 783,
		48, 779,
		48, 771
	};
	size = 20;
	CreateChain(x, y, leftWall, size, b2_staticBody);

	int rightWall[20] = {
		431, 686,
		439, 686,
		439, 778,
		437, 783,
		347, 824,
		342, 822,
		341, 816,
		429, 775,
		430, 769,
		430, 750
	};
	size = 20;
	CreateChain(x, y, rightWall, size, b2_staticBody);

	int mapCollider1[102] = {
	71, 390,
	60, 336,
	53, 285,
	56, 220,
	75, 162,
	108, 112,
	141, 83,
	185, 55,
	223, 44,
	246, 63,
	259, 80,
	259, 112,
	260, 124,
	275, 153,
	277, 243,
	274, 252,
	269, 253,
	183, 318,
	181, 324,
	176, 326,
	172, 323,
	172, 318,
	158, 295,
	152, 273,
	154, 259,
	160, 246,
	170, 238,
	172, 230,
	171, 219,
	165, 212,
	153, 209,
	139, 217,
	128, 230,
	120, 251,
	118, 277,
	121, 293,
	129, 317,
	135, 330,
	142, 338,
	143, 345,
	143, 352,
	137, 354,
	130, 346,
	121, 329,
	108, 303,
	63, 319,
	68, 344,
	78, 385,
	79, 401,
	75, 402,
	71, 398
	};
	size = 102;
	CreateChain(x, y, mapCollider1, size, b2_staticBody);

	int mapCollider2[58] = {
		297, 336,
		298, 345,
		305, 346,
		307, 339,
		314, 334,
		328, 331,
		340, 335,
		346, 345,
		345, 357,
		341, 366,
		342, 374,
		347, 379,
		354, 379,
		372, 328,
		413, 341,
		399, 407,
		404, 410,
		409, 407,
		409, 402,
		445, 360,
		451, 359,
		451, 347,
		423, 298,
		391, 253,
		357, 229,
		328, 211,
		324, 213,
		316, 251,
		307, 292
	};
	size = 58;
	CreateChain(x, y, mapCollider2, size, b2_staticBody);

	int mapCollider3[50] = {
		464, 305,
		468, 296,
		480, 262,
		481, 225,
		474, 182,
		455, 138,
		438, 112,
		410, 85,
		400, 78,
		392, 78,
		385, 82,
		384, 90,
		382, 152,
		381, 195,
		382, 201,
		384, 202,
		388, 202,
		394, 208,
		415, 232,
		434, 261,
		448, 288,
		455, 300,
		458, 305,
		461, 307,
		463, 307
	};
	size = 50;
	CreateChain(x, y, mapCollider3, size, b2_staticBody);

	int Pills[16] = {
		335, 80,
		340, 74,
		349, 74,
		353, 80,
		353, 120,
		349, 124,
		339, 124,
		335, 120
	};
	size = 16;
	CreateChain(x - 1, y, Pills, size, b2_staticBody);
	CreateChain(x - 46, y + 5, Pills, size, b2_staticBody);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 0.65f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = 0.4f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

/*b2MouseJoint* ModulePhysics::CreateMouseJoint(b2Body* body_clicked, b2Vec2 mouse_position)
{
	b2Vec2 mouse_position;
	mouse_position.x = App->input->GetMouseX();
	mouse_position.y = App->input->GetMouseY();
	b2MouseJointDef def;
	def.bodyA = ground;
	def.bodyB = body_clicked;
	def.target = mouse_position;
	def.dampingRatio = 0.5f;
	def.frequencyHz = 2.0f;
	def.maxForce = 100.0f * body_clicked->GetMass();
	mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

	return mouse_joint;
}*/

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}


			// TODO 1: If mouse button 1 is pressed ...
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				// test if the current body contains mouse position
				b2Vec2 vec(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
				if (f->TestPoint(vec)) 
				{			
					// TODO 2: If a body was selected, create a mouse joint
					// using mouse_joint class property

					b2MouseJointDef def;
					def.bodyA = ground;
					def.bodyB = b;
					def.target = vec;
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * b->GetMass();
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}

			// TODO 3: If the player keeps pressing the mouse button, update
			// target position and draw a red line between both anchor points

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
			{
				mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()),PIXEL_TO_METERS(App->input->GetMouseY()) });
				b2Vec2 A;
				b2Vec2 B;
				A.x = METERS_TO_PIXELS(mouse_joint->GetAnchorA().x);
				A.y = METERS_TO_PIXELS(mouse_joint->GetAnchorA().y);
				B.x = METERS_TO_PIXELS(mouse_joint->GetAnchorB().x);
				B.y = METERS_TO_PIXELS(mouse_joint->GetAnchorB().y);
				App->renderer->DrawLine(A.x, A.y, B.x, B.y, 255, 0, 0);
			}
			// TODO 4: If the player releases the mouse button, destroy the joint

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
			{
				world->DestroyJoint(mouse_joint);
				mouse_joint = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}