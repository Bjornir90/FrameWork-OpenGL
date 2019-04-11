#include "PhysicObject.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


PhysicObject::PhysicObject(glm::vec4* position, double width, double height, double length, double mass, bool isAnchor):m_position(position), m_width(width), m_height(height), m_length(length), m_speed(glm::vec4(0.0)), m_mass(mass), m_isAnchor(isAnchor)
{
	m_forces = new std::vector<Force>();
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::ApplyForce(Force f)
{
	m_forces->push_back(f);
}

void PhysicObject::Update(float delta)
{
	if (this->m_isAnchor) {
		this->m_speed = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	Force* total = new Force(glm::vec4(0), glm::vec4(0));
	for (std::vector<int>::size_type i = 0; i < m_forces->size(); i++) {
		total = total->AddForce(&(m_forces->at(i)));
	}
	//Remove all applied forces so applied forces can change over time
	m_forces->clear();
	m_speed = m_speed + total->MultiplyByScalar(delta).GetDirection();
	*m_position = *m_position + m_speed*delta;
}

std::vector<glm::vec4> PhysicObject::GetCornersPos()
{
	std::vector<glm::vec4> corners = std::vector<glm::vec4>();
	glm::vec4 corner(m_position->x, m_position->y, m_position->z, 1.0);
	corners.push_back(corner);
	std::cout << "Error : operation not yet supported" << std::endl;
	return corners;
}

bool PhysicObject::CollidesWith(PhysicObject *that, Sides *collisionSide){
	//These are the coordinates for each sides
	float thisLeft = this->m_position->x, thatRight = that->m_position->x + that->m_width, thisRight = this->m_position->x + this->m_width, thatLeft = that->m_position->x;
	float thisTop = this->m_position->y, thatBot = that->m_position->y + that->m_height, thisBot = this->m_position->y + this->m_height, thatTop = that->m_position->y;
	float thisFront = this->m_position->z, thatBehind = that->m_position->z + that->m_length, thisBehind = this->m_position->z + this->m_length, thatFront = that->m_position->z;
	
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

void PhysicObject::OnCollision(PhysicObject *that, Sides * collisionSide){
	Sides side = *collisionSide;
	glm::vec3 normalToFace;
	glm::vec3 thisSpeed = glm::vec3(this->m_speed), thatSpeed = glm::vec3(that->m_speed); 
	std::cout << "Speed at collision : " << glm::to_string(thisSpeed) << " other speed : " << glm::to_string(thatSpeed) << std::endl;
	std::cout << "Position at collision : " << glm::to_string(*m_position) << " other position : " << glm::to_string(*(that->m_position)) << std::endl;
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
		this->m_speed = glm::vec4(thisSpeed, 0.0f);
		this->Update(2.0f);
		return;
	}

	float thatNormalComponent = glm::dot(normalToFace, thatSpeed);

	thisSpeed = (-2.0f * thisNormalComponent * normalToFace + thisSpeed) * (float) (that->m_mass/(this->m_mass+that->m_mass));
	thatSpeed = (-2.0f * thatNormalComponent * normalToFace + thatSpeed) * (float) (this->m_mass / (this->m_mass + that->m_mass));

	glm::vec3 distanceCenterToCenter = this->GetCenter() - that->GetCenter();


	this->m_speed = glm::vec4(thisSpeed, 0.0f);
	that->m_speed = glm::vec4(thatSpeed, 0.0f);
	this->Update(2.0f);
}

glm::vec4 PhysicObject::GetSpeed()
{
	return m_speed;
}

void PhysicObject::SetSpeed(glm::vec4 speed)
{
	m_speed = speed;
}

void PhysicObject::SetSpeed(glm::vec3 speed)
{
	m_speed = glm::vec4(speed, 1.0f);
}

glm::vec4 PhysicObject::GetPos()
{
	return *m_position;
}

glm::vec3 PhysicObject::GetCenter() {
	glm::vec3 center = glm::vec3(this->m_position->x+this->m_length/2.0f, this->m_position->y+this->m_width/2.0f, this->m_position->z+this->m_height/2.0f);
	return center;
}
