#include "TravelingSalesmanSolver.h"
#include "TravelingSalesmanAlgorithms.h"

#include <random>
#include <limits>

TravelingSalesmanSolver::TravelingSalesmanSolver(int numberOfPoints, float xMin, float xMax, float yMin, float yMax)
{
    m_xMin = xMin;
    m_xMax = xMax;
    m_yMin = yMin;
    m_yMax = yMax;
	generatePoints(numberOfPoints);
}

TravelingSalesmanSolver::~TravelingSalesmanSolver()
{
    m_isInterrupt = true;
    if (m_solving.joinable())
    {
        m_solving.join();
    }
}

void TravelingSalesmanSolver::generatePoints(int numberOfPoints)
{
	std::random_device rd;
	std::uniform_real_distribution<double> xDistribution(m_xMin, m_xMax);
	std::uniform_real_distribution<double> yDistribution(m_yMin, m_yMax);

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

    TravelingSalesmanAlgorithms::greedyAlgorithm(m_timeStepNanoseconds, m_isInterrupt, m_points, m_route);

    m_isSolving = false;
}
