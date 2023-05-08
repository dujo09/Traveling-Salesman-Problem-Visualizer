#include "TravelingSalesmanSolver.h"

#include <random>
#include <limits>

TravelingSalesmanSolver::TravelingSalesmanSolver(int numberOfPoints, int xMin, int xMax, int yMin, int yMax)
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

void TravelingSalesmanSolver::generatePoints(int numberOfPoints, int xMin, int xMax, int yMin, int yMax)
{
	std::random_device rd;
	std::uniform_real_distribution<double> xDistribution(xMin, xMax);
	std::uniform_real_distribution<double> yDistribution(yMin, yMax);

    m_points.clear();

    for (int i = 0; i < numberOfPoints; ++i)
    {
        float x = xDistribution(rd);
        float y = yDistribution(rd);

        m_points.emplace_back(x, y);
    }
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
        Point2D* closestNeighbour = nullptr;

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
                closestNeighbour = &m_points.at(i);
                isVisited.at(i) = true;
            }
        }
        m_route.push_back(closestNeighbour);
    }

    m_route.push_back(&m_points.at(startPointIndex));
}
