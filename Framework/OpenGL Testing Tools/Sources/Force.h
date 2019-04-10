#pragma once
#include "glm/glm.hpp"

class Force
{
private:
	glm::vec4 m_direction, m_applicationPoint;
public:
	Force(glm::vec4 direction, glm::vec4 applicationPoint);
	~Force();
	Force* AddForce(Force* other);

	glm::vec4 GetDirection() {
		return m_direction;
	}

	glm::vec4* GetPointerToDirection() {
		return &m_direction;
	}

	glm::vec4 GetApplicationPoint() {
		return m_applicationPoint;
	}

	void MoveTo(glm::vec4 newApplicationPoint);
	void MoveBy(glm::vec4 delta);

	Force MultiplyByScalar(float k);
	Force MultiplyByScalar(int k);
	void MultiplySelfByScalar(float k);
	Force MultiplyByMatrix(glm::mat4 mat);

	glm::mat4 ApplyToModel(glm::mat4 model);
};

