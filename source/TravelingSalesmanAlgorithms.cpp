#include "TravelingSalesmanAlgorithms.h"
#include "SolverColors.h"

#include <random>
#include <limits>
#include <thread>

void TravelingSalesmanAlgorithms::greedyAlgorithm(const unsigned int& timeStepMilliseconds, const std::atomic<bool>& isInterrupt, 
    std::vector<Point2D>& points, std::vector<Point2D*>& route, float& routeLength)
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

    int startPointIndex = startPointDistribution(rd);
    std::vector<bool> isVisited(points.size(), false);

    startPointIndex = 0;

    isVisited.at(startPointIndex) = true;
    route.push_back(&points.at(startPointIndex));

    //points.at(startPointIndex).setColor(SolverColors::POINT_HIGHLIGHT_COLOR);
    points.at(startPointIndex).setOutgoingLineColor(SolverColors::LINE_HIGHLIGHT_COLOR);

    while (route.size() < points.size())
    {
        if (isInterrupt)
        {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));

        Point2D* currentPoint = route.back();
        int closestPointIndex = -1;
        int numberOfPoints = points.size();

        float minDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < numberOfPoints; ++i)
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

        points.at(closestPointIndex).setOutgoingLineColor(SolverColors::LINE_HIGHLIGHT_COLOR);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));

    routeLength += route.back()->getDistanceToPoint(points.at(startPointIndex));
    route.push_back(&points.at(startPointIndex));

    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds)); 
}

void TravelingSalesmanAlgorithms::twoOptAlgorithm(const unsigned int& timeStepMilliseconds, const std::atomic<bool>& isInterrupt, 
    std::vector<Point2D>& points, std::vector<Point2D*>& route, float& routeLength)
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


    greedyAlgorithm(timeStepMilliseconds, isInterrupt, points, route, routeLength);

    int numberOfPoints = points.size();
    bool foundImprovement = true;

    while (foundImprovement)
    {
        foundImprovement = false;
        for (int i = 0; i <= numberOfPoints - 2; ++i)
        {
            for (int j = i + 2; j <= numberOfPoints - 1; ++j)
            {
                if (isInterrupt)
                {
                    return;
                }

                Point2D* firstEdge[2] = { route.at(i), route.at((i + 1) % numberOfPoints) };
                Point2D* secondEdge[2] = { route.at(j), route.at((j + 1) % numberOfPoints) };
                
                firstEdge[0]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                secondEdge[0]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));
                firstEdge[0]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);
                secondEdge[0]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);

                std::reverse(route.begin() + i + 1, route.begin() + j + 1);

                firstEdge[0]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                firstEdge[1]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));
                firstEdge[0]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);
                firstEdge[1]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);

                // Formula: https://en.wikipedia.org/wiki/2-opt
                float routeLengthDelta = firstEdge[0]->getDistanceToPoint(*secondEdge[0]) + firstEdge[1]->getDistanceToPoint(*secondEdge[1]) 
                    - firstEdge[0]->getDistanceToPoint(*firstEdge[1]) - secondEdge[0]->getDistanceToPoint(*secondEdge[1]);

                if ((int)routeLengthDelta >= 0)
                {
                    std::reverse(route.begin() + i + 1, route.begin() + j + 1);

                    firstEdge[0]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                    secondEdge[0]->setOutgoingLineColor(SolverColors::POINT_HIGHLIGHT_COLOR);
                    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMilliseconds));
                    firstEdge[0]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);
                    secondEdge[0]->setOutgoingLineColor(SolverColors::DEFAULT_COLOR);
                }
                else
                {
                    routeLength += routeLengthDelta;
                    foundImprovement = true;
                }
            }
        }
    }
}
