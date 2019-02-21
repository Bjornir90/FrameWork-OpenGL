#include "Force.h"
#include "glm/gtc/matrix_transform.hpp"



Force::Force(glm::vec4 direction, glm::vec4 applicationPoint): direction(direction), applicationPoint(applicationPoint)
{
}

Force::~Force()
{
}

Force * Force::addForce(Force * other)
{
	glm::vec4 sum = other->getDirection() + direction;
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

Force Force::multiplyByScalar(float k)
{
	glm::vec4 newDirection = direction * k;
	Force result(newDirection, applicationPoint);
	return result;
}

glm::mat4 Force::applyToModel(glm::mat4 model)
{
	glm::mat4 result = glm::translate(model, glm::vec3(direction));
	return result;
}

