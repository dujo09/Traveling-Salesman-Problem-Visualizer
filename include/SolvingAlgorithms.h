#pragma once

#include "Point2D.h"

#include <vector>
#include <atomic>

class SolvingAlgorithms
{
public:
	static void greedyAlgorithm(const unsigned int& timeStepNanoseconds, const std::atomic<bool>& isInterrupt, 
		std::vector<Point2D>& points, std::vector<Point2D*>& route);
};

