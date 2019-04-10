#include "Force.h"
#include "glm/gtc/matrix_transform.hpp"



Force::Force(glm::vec4 direction, glm::vec4 applicationPoint): m_direction(direction), m_applicationPoint(applicationPoint)
{
}

Force::~Force()
{
}

Force* Force::AddForce(Force* other)
{
	glm::vec4 sum = other->GetDirection() + m_direction;
	//two added forces should have the same application point, might change when moments are added
	return new Force(sum, m_applicationPoint);
}

void Force::MoveTo(glm::vec4 newApplicationPoint)
{
	m_applicationPoint = newApplicationPoint;
}

void Force::MoveBy(glm::vec4 delta)
{
	m_applicationPoint += delta;
}

Force Force::MultiplyByScalar(float k)
{
	glm::vec4 newDirection = m_direction * k;
	Force result(newDirection, m_applicationPoint);
	return result;
}

Force Force::MultiplyByMatrix(glm::mat4 mat)
{
	glm::vec4 newDirection = mat * m_direction;
	Force result(newDirection, m_applicationPoint);
	return result;
}

glm::mat4 Force::ApplyToModel(glm::mat4 model)
{
	glm::mat4 result = glm::translate(model, glm::vec3(m_direction));
	return result;
}

