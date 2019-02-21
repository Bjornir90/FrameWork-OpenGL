#include "PhysicObject.h"



PhysicObject::PhysicObject(glm::vec4 position, double width, double height, double length):position(position), width(width), height(height), length(length), speed(glm::vec4(0.0))
{
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::applyForce(Force f)
{
	forces.push_back(f);
}

void PhysicObject::update(float delta)
{
	Force *total = new Force(glm::vec4(0), glm::vec4(0));
	for (auto i = forces.begin(); i != forces.end(); i++) {
		total = i->addForce(total);
	}
	speed = speed + total->multiplyByScalar(delta).getDirection();
	position = position + speed*delta;
}

std::vector<glm::vec4> PhysicObject::getCornersPos()
{
	std::vector<glm::vec4> corners = std::vector<glm::vec4>();
	glm::vec4 corner(position.x - width / 2.0, position.y - height / 2.0, position.z - length / 2.0, 1.0);
	corners.push_back(corner);
}
