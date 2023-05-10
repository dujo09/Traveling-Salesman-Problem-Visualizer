#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Point2D
{
	float m_x;
	float m_y;
	glm::vec3 m_color;

public:
	Point2D(float x, float y);

	float getDistanceToPoint(const Point2D& other) const;
	float getSquaredDistanceToPoint(const Point2D& other) const;

	float getX() const { return m_x; };
	void setX(float x) { m_x = x; };
	float getY() const { return m_y; };
	void setY(float y) { m_y = y; };
	glm::vec3 getColor() const { return m_color; };
	void setColor(glm::vec3 color) { m_color = color; };
};

