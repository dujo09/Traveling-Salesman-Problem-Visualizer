#pragma once

#include "Point2D.h"

#include <vector>
#include <atomic>

class TravelingSalesmanAlgorithms
{
public:
	static void greedyAlgorithm(const unsigned int& timeStepMiliseconds, const std::atomic<bool>& isInterrupt, 
		std::vector<Point2D>& points, std::vector<Point2D*>& route, int& routeLength);
};

