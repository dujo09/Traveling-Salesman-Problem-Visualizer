#include "SolvingAlgorithms.h"

#include <random>
#include <limits>
#include <thread>

void SolvingAlgorithms::greedyAlgorithm(const unsigned int& timeStepNanoseconds, const std::atomic<bool>& isInterrupt, 
    std::vector<Point2D>& points, std::vector<Point2D*>& route)
{
    if (!route.empty())
    {
        route.clear();
    }

    std::vector<bool> isVisited(points.size(), false);

    std::random_device rd;
    std::uniform_int_distribution<int> distribution(0, points.size() - 1);

    const int startPointIndex = distribution(rd);
    route.push_back(&points.at(startPointIndex));
    isVisited.at(startPointIndex) = true;

    while (route.size() < points.size())
    {
        if (isInterrupt)
        {
            return;
        }

        Point2D* currentPoint = route.back();
        int closestNeighbourIndex = -1;

        float minSquaredDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < points.size(); ++i)
        {
            if (isVisited.at(i))
            {
                continue;
            }

            const float squaredDistance = currentPoint->getSquaredDistanceToPoint(points.at(i));

            if (squaredDistance < minSquaredDistance)
            {
                minSquaredDistance = squaredDistance;
                closestNeighbourIndex = i;

            }
        }
        // TODO throw exception if closestNeighbourIndex is -1
        route.push_back(&points.at(closestNeighbourIndex));
        isVisited.at(closestNeighbourIndex) = true;

        std::this_thread::sleep_for(std::chrono::nanoseconds(timeStepNanoseconds));
    }

    route.push_back(&points.at(startPointIndex));
}
