#pragma once
#include "glm/glm.hpp"
#include "Force.h"
#include <vector>

class PhysicObject
{
private:
	glm::vec4 position, speed;
	double width, height, length;//x, y and z axis dimension of the axis aligned bounding box
	std::vector<Force> forces;
public:
	PhysicObject(glm::vec4 position, double width, double height, double length);
	~PhysicObject();
	void applyForce(Force f);
	void update(float delta);
	std::vector<glm::vec4> getCornersPos();
};

