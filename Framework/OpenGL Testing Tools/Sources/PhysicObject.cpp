#include "PhysicObject.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


PhysicObject::PhysicObject(glm::vec4 position, double width, double height, double length, double mass):position(position), width(width), height(height), length(length), speed(glm::vec4(0.0)), mass(mass)
{
	forces = new std::vector<Force>();
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::applyForce(Force f)
{
	forces->push_back(f);
}

void PhysicObject::update(float delta)
{
	Force *total = new Force(glm::vec4(0), glm::vec4(0));
	/*for (auto i = forces->begin(); i != forces->end(); i++) {
		total = i->addForce(total);
		forces->erase(i);
	}*/
	for (std::vector<int>::size_type i = 0; i < forces->size(); i++) {
		total = total->addForce(&(forces->at(i)));
		std::cout << i << std::endl;
	}
	//Remove all applied forces so applied forces can change over time
	forces->clear();
	speed = speed + total->multiplyByScalar(delta).getDirection();
	position = position + speed*delta;
}

std::vector<glm::vec4> PhysicObject::getCornersPos()
{
	std::vector<glm::vec4> corners = std::vector<glm::vec4>();
	glm::vec4 corner(position.x, position.y, position.z, 1.0);
	corners.push_back(corner);
	std::cout << "Error : operation not yet supported" << std::endl;
	return corners;
}

bool PhysicObject::collidesWith(PhysicObject *that){
	if(this->position.x < that->position.x+that->width && this->position.x+this->width > that->position.x){//horizontal check
		if(this->position.y < that->position.y+that->height && this->position.y+this->height > that->position.y) {//vertical check
			if(this->position.z < that->position.z+that->length && this->position.z+this->length > that->position.z) {//depth check
				std::cout << "Collision" << std::endl;
				return true;
			}
		}
	}
	return false;
}

void PhysicObject::onCollision(PhysicObject *that){
	glm::vec4 thatMomentum = that->speed*(float)that->mass;
	glm::vec4 thisMomentum = this->speed*(float)this->mass;
	glm::vec3 bisector = glm::normalize(glm::normalize(that->speed) + glm::normalize(this->speed));
	//ATTENTION : this will cause nan computation if the two speeds have the exact same direction
	glm::vec3 normalToCollisionPlane = glm::cross(glm::vec3(that->speed), glm::vec3(this->speed));
	glm::vec3 normalToBisector = glm::cross(bisector, normalToCollisionPlane);
	bisector = glm::normalize(bisector);
	normalToBisector = glm::normalize(normalToBisector);
	std::cout << glm::to_string(bisector) << glm::to_string(normalToCollisionPlane) << std::endl;
	//Now bisector and its normal are a base of the collision plane
	//We project the two speed vectors to the normal vector
	float thisNormalSpeedComponent = glm::dot(glm::vec3(this->speed), normalToBisector);
	float thatNormalSpeedComponent = glm::dot(glm::vec3(that->speed), normalToBisector);
	float thisOtherSpeedComponent = glm::dot(glm::vec3(this->speed), bisector);
	float thatOtherSpeedComponent = glm::dot(glm::vec3(that->speed), bisector);
	//Reverse the speed components normal to the collision
	//Masses are inversed because the biggest mass gives the most speed to the other object
	glm::vec3 thisNewSpeed = (-2 * thisNormalSpeedComponent * normalToBisector + thisOtherSpeedComponent * bisector) * (float)(that->mass / (that->mass + this->mass));
	glm::vec3 thatNewSpeed = (-2 * thatNormalSpeedComponent * normalToBisector + thatOtherSpeedComponent * bisector) * (float)(this->mass / (that->mass + this->mass));
	std::cout << "New speed of this : " << glm::to_string(thisNewSpeed) << " new speed of that : " << glm::to_string(thatNewSpeed) << std::endl;
	this->speed = glm::vec4(thisNewSpeed, 1.0f);
	that->speed = glm::vec4(thatNewSpeed, 1.0f);
	position = position + speed*10.0f;
	that->position = that->position + that->speed*10.0f;
}

glm::vec4 PhysicObject::getSpeed()
{
	return speed;
}

glm::vec4 PhysicObject::getPos()
{
	return position;
}
