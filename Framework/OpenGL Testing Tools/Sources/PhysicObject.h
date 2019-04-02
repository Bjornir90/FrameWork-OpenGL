#pragma once
#include "glm/glm.hpp"
#include "Force.h"
#include <vector>

enum Sides {LEFTSIDE, RIGHTSIDE, TOP, BOT, FRONT, BEHIND};

class PhysicObject
{
private:
	glm::vec4 position, speed;//position is left corner, for collisions
	double width, height, length, mass;//x, y and z axis dimension of the axis aligned bounding box
	bool isAnchor;
	std::vector<Force> *forces;
public:
	PhysicObject(glm::vec4 position, double width, double height, double length, double mass, bool isAnchor);
	~PhysicObject();
	void applyForce(Force f);
	void update(float delta);
	std::vector<glm::vec4> getCornersPos();
    bool collidesWith(PhysicObject *that, Sides * collisionSide);
    void onCollision(PhysicObject *that, Sides * collisionSide);
	glm::vec4 getSpeed();
	glm::vec4 getPos();
};

