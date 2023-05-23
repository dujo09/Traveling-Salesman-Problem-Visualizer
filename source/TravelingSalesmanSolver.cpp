#include "TravelingSalesmanSolver.h"
#include "TravelingSalesmanAlgorithms.h"
#include "SolverColors.h"

#include <random>
#include <limits>

int TravelingSalesmanSolver::MAX_TIME_STEP_MILLISECONDS = 1000;

TravelingSalesmanSolver::TravelingSalesmanSolver(int numberOfPoints, float xMin, float xMax, float yMin, float yMax)
{
    generatePoints(numberOfPoints, xMin, xMax, yMin, yMax);
}

TravelingSalesmanSolver::~TravelingSalesmanSolver()
{
    m_isInterrupt = true;
    if (m_solving.joinable())
    {
        m_solving.join();
    }
}

void TravelingSalesmanSolver::generatePoints(int numberOfPoints, float xMin, float xMax, float yMin, float yMax)
{
    if (m_isSolving)
    {
        return;
    }

	std::random_device rd;
	std::uniform_real_distribution<double> xDistribution(xMin, xMax);
	std::uniform_real_distribution<double> yDistribution(yMin, yMax);

    m_route.clear();
    m_points.clear();

    for (int i = 0; i < numberOfPoints; ++i)
    {
        float x = xDistribution(rd);
        float y = yDistribution(rd);

        m_points.emplace_back(x, y);
    }

    m_route.reserve(numberOfPoints + 1);
}

void TravelingSalesmanSolver::startSolving()
{
    if (m_isSolving)
    {
        return;
    }

    if (m_solving.joinable())
    {
        m_solving.join();
    }
    m_solving = std::thread(&TravelingSalesmanSolver::_startSolving, this);
}

void TravelingSalesmanSolver::_startSolving()
{
    m_isSolving = true;
    m_isInterrupt = false;

    TravelingSalesmanAlgorithms::twoOptAlgorithm(m_timeStepMilliseconds, m_isInterrupt, m_points, m_route, m_routeLength);

    setWholeRouteColor(SolverColors::COMPLETE_COLOR);

    m_isSolving = false;
}

void TravelingSalesmanSolver::setAllPointsColors(glm::vec3 color)
{
    for (Point2D& point : m_points)
    {
        point.setColor(color);
    }
}

void TravelingSalesmanSolver::setWholeRouteColor(glm::vec3 color)
{
    for (Point2D& point : m_points)
    {
        point.setOutgoingLineColor(color);
    }
}
