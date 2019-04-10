#pragma once
#include "glm/glm.hpp"
#include "Force.h"
#include <vector>

enum Sides {LEFTSIDE, RIGHTSIDE, TOP, BOT, FRONT, BEHIND};

class PhysicObject
{
private:
	glm::vec4* m_position;
	glm::vec4 m_speed;//position is left corner, for collisions
	double m_width, m_height, m_length, m_mass;//x, y and z axis dimension of the axis aligned bounding box
	bool m_isAnchor;
	std::vector<Force>* m_forces;
public:
	PhysicObject(glm::vec4* position, double width, double height, double length, double mass, bool isAnchor);
	~PhysicObject();
	void ApplyForce(Force f);
	void Update(float delta);
	std::vector<glm::vec4> GetCornersPos();
    bool CollidesWith(PhysicObject* that, Sides* collisionSide);
    void OnCollision(PhysicObject* that, Sides* collisionSide);
	glm::vec4 GetSpeed();
	glm::vec4 GetPos();
	glm::vec3 GetCenter();
};

