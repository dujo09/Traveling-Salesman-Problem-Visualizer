#include "Point2D.h"
#include "SolverColors.h"

#include <cmath>

Point2D::Point2D(float x, float y)
{
    m_x = x;
    m_y = y;
    m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_outgoingLineColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

float Point2D::getDistanceToPoint(const Point2D& other) const
{
    return sqrt(pow(other.getX() - this->getX(), 2) + pow(other.getY() - this->getY(), 2));
}

float Point2D::getSquaredDistanceToPoint(const Point2D& other) const
{
    return pow(other.getX() - this->getX(), 2) + pow(other.getY() - this->getY(), 2);
}
