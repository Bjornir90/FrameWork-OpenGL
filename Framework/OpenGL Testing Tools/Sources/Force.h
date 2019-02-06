#pragma once
#include "glm/glm.hpp"

class Force
{
private:
	glm::vec4 direction, applicationPoint;
public:
	Force(glm::vec4 direction, glm::vec4 applicationPoint);
	~Force();
	Force *addForce(Force *other);

	glm::vec4 getDirection() {
		return direction;
	}

	glm::vec4 getApplicationPoint() {
		return applicationPoint;
	}

	void moveTo(glm::vec4 newApplicationPoint);
	void moveBy(glm::vec4 delta);
};

