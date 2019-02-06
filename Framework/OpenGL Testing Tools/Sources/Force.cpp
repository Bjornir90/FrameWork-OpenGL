#include "Force.h"





Force::Force(glm::vec4 direction, glm::vec4 applicationPoint): direction(direction), applicationPoint(applicationPoint)
{
}

Force::~Force()
{
}

Force * Force::addForce(Force * other)
{
	glm::vec4 sum = other->getDirection + direction;
	//two added forces should have the same application point, might change when moments are added
	return new Force(sum, applicationPoint);
}

void Force::moveTo(glm::vec4 newApplicationPoint)
{
	applicationPoint = newApplicationPoint;
}

void Force::moveBy(glm::vec4 delta)
{
	applicationPoint += delta;
}

