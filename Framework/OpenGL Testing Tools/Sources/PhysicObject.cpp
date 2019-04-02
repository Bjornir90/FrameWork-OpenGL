#include "PhysicObject.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


PhysicObject::PhysicObject(glm::vec4 position, double width, double height, double length, double mass, bool isAnchor):position(position), width(width), height(height), length(length), speed(glm::vec4(0.0)), mass(mass), isAnchor(isAnchor)
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
	if (this->isAnchor) {
		this->speed = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	Force *total = new Force(glm::vec4(0), glm::vec4(0));
	for (std::vector<int>::size_type i = 0; i < forces->size(); i++) {
		total = total->addForce(&(forces->at(i)));
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

bool PhysicObject::collidesWith(PhysicObject *that, Sides *collisionSide){
	//These are the coordinates for each sides
	float thisLeft = this->position.x, thatRight = that->position.x + that->width, thisRight = this->position.x + this->width, thatLeft = that->position.x;
	float thisTop = this->position.y, thatBot = that->position.y + that->height, thisBot = this->position.y + this->height, thatTop = that->position.y;
	float thisFront = this->position.z, thatBehind = that->position.z + that->length, thisBehind = this->position.z + this->length, thatFront = that->position.z;
	
	if(thisLeft < thatRight && thisRight > thatLeft){//horizontal check
		if(thisTop < thatBot && thisBot > thatTop) {//vertical check
			if(thisFront < thatBehind && thisBehind > thatFront) {//depth check
				std::cout << "Collision" << std::endl;

				//Now calculates distance to each sides, to determine on which the collision has occured
				//All distances are from the indicated sides on this object
				float distances[] = {/*leftDistance = */ abs(thisLeft - thatRight), /*rightDistance = */abs(thisRight - thatLeft), /*topDistance = */abs(thisTop - thatBot), /*botDistance = */abs(thisBot - thatTop), /*frontDistance = */abs(thisFront - thatBehind), /*behindDistance =*/ abs(thisBehind - thatFront) };
				int indexOfSmallestDistance = 0;

				for (int i = 1; i < 6; i++) {
					if (distances[i] < distances[indexOfSmallestDistance]) indexOfSmallestDistance = i;
				}

				//Same order of sides in distances and Sides enum
				std::cout << "on side " << indexOfSmallestDistance << std::endl;
				*collisionSide = (Sides)indexOfSmallestDistance;
				return true;
			}
		}
	}
	return false;
}

void PhysicObject::onCollision(PhysicObject *that, Sides * collisionSide){
	Sides side = *collisionSide;
	glm::vec3 normalToFace;
	glm::vec3 thisSpeed = glm::vec3(this->speed), thatSpeed = glm::vec3(that->speed); 
	if (side == LEFTSIDE) normalToFace = glm::vec3(-1.0f, 0.0f, 0.0f);
	else if(side == RIGHTSIDE) normalToFace = glm::vec3(1.0f, 0.0f, 0.0f);
	else if (side == TOP) normalToFace = glm::vec3(0.0f, -1.0f, 0.0f);
	else if (side == BOT) normalToFace = glm::vec3(0.0f, 1.0f, 0.0f);
	else if (side == FRONT) normalToFace = glm::vec3(0.0f, 0.0f, -1.0f);
	else if (side == BEHIND) normalToFace = glm::vec3(0.0f, 0.0f, 1.0f);

	float thisNormalComponent = glm::dot(normalToFace, thisSpeed);

	//If the other object is immobile, simply reverse the normal component of this speed
	if (thatSpeed == glm::vec3(0.0f)) {
		thisSpeed = -2 * thisNormalComponent * normalToFace + thisSpeed;
		std::cout << "New this speed : " << glm::to_string(thisSpeed) << "this normal component : " << thisNormalComponent << std::endl;
		this->speed = glm::vec4(thisSpeed, 0.0f);
		this->update(2.0f);
		return;
	}

	float thatNormalComponent = glm::dot(normalToFace, thatSpeed);

	thisSpeed = (-2.0f * thisNormalComponent * normalToFace + thisSpeed) * (float) (that->mass/(this->mass+that->mass));
		thatSpeed = (-2.0f * thatNormalComponent * normalToFace + thatSpeed) * (float) (this->mass / (this->mass + that->mass));

	this->speed = glm::vec4(thisSpeed, 0.0f);
	that->speed = glm::vec4(thatSpeed, 0.0f);
	this->update(2.0f);
}

glm::vec4 PhysicObject::getSpeed()
{
	return speed;
}

glm::vec4 PhysicObject::getPos()
{
	return position;
}
