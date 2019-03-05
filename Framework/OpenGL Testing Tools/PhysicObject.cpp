#include "PhysicObject.h"



PhysicObject::PhysicObject(glm::vec4 position, double width, double height, double length, double mass):position(position), width(width), height(height), length(length), speed(glm::vec4(0.0)), mass(mass)
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

bool PhysicObject::collidesWith(PhysicObject *that){
	if(this->position.x < that->position.x+that->width && this->position.x+this->width > that->position.x){//horizontal check
		if(this->position.y < that->position.y+that->height && this->position.y+this->height > that->position.y) {//vertical check
			if(this->position.z < that->position.z+that->length && this->position.z+this->length > that->position.z) {//depth check
				return true;
			}
		}
	}
	return false;
}

void PhysicObject::onCollision(PhysicObject *that){
	glm::vec4 thatMomentum = that->speed*that->mass;
	glm::vec4 thisMomentum = this->speed*this->mass;
	glm::vec4 sumMomentum = thatMomentum+thisMomentum;
	glm::vec4 thatAfterCollisionMomentum = thatMomentum-sumMomentum;
	glm::vec4 thisAfterCollisionMomentum = thisMomentum-sumMomentum;

}