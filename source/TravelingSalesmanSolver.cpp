#include "TravelingSalesmanSolver.h"

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

    greedyAlgorithm();

    m_isSolving = false;
}

void TravelingSalesmanSolver::greedyAlgorithm()
{
    m_route.clear();
    std::vector<bool> isVisited(m_points.size(), false);

    std::random_device rd;
    std::uniform_int_distribution<int> distribution(0, m_points.size() - 1);
    
    const int startPointIndex = distribution(rd);
    m_route.push_back(&m_points.at(startPointIndex));
    isVisited.at(startPointIndex) = true;

    while (m_route.size() < m_points.size())
    {
        Point2D* currentPoint = m_route.back();
        int closestNeighbourIndex = -1;

        float minSquaredDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < m_points.size(); ++i)
        {
            if (isVisited.at(i))
            {
                continue;
            }

            const float squaredDistance = currentPoint->getSquaredDistanceToPoint(m_points.at(i));

            if (squaredDistance < minSquaredDistance)
            {
                minSquaredDistance = squaredDistance;
                closestNeighbourIndex = i;
                
            }
        }
        m_route.push_back(&m_points.at(closestNeighbourIndex));
        isVisited.at(closestNeighbourIndex) = true;
    }

    m_route.push_back(&m_points.at(startPointIndex));
}
