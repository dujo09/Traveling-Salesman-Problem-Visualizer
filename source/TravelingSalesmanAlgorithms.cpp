#include "TravelingSalesmanAlgorithms.h"

#include <random>
#include <limits>
#include <thread>

void TravelingSalesmanAlgorithms::greedyAlgorithm(const unsigned int& timeStepMilliseconds, const std::atomic<bool>& isInterrupt, 
    std::vector<Point2D>& points, std::vector<Point2D*>& route, int& routeLength)
{
    if (!route.empty())
    {
        route.clear();
    }
    if (route.capacity() < points.size() + 1)
    {
        route.reserve(points.size() + 1);
    }

    routeLength = 0;


    std::random_device rd;
    std::uniform_int_distribution<int> startPointDistribution(0, points.size() - 1);

    const int startPointIndex = startPointDistribution(rd);
    std::vector<bool> isVisited(points.size(), false);

    isVisited.at(startPointIndex) = true;
    route.push_back(&points.at(startPointIndex));
    //std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));
    
    while (route.size() < points.size())
    {
        if (isInterrupt)
        {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));

        Point2D* currentPoint = route.back();
        int closestPointIndex = -1;

        float minDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < points.size(); ++i)
        {
            if (isVisited.at(i))
            {
                continue;
            }

            const float distance = currentPoint->getDistanceToPoint(points.at(i));

            if (distance < minDistance)
            {
                minDistance = distance;
                closestPointIndex = i;
            }
        }
        // TODO throw exception if closestNeighbourIndex is -1
        route.push_back(&points.at(closestPointIndex));
        isVisited.at(closestPointIndex) = true; 
        routeLength += minDistance;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));
    route.push_back(&points.at(startPointIndex));
}
