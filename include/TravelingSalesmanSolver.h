#pragma once

#include "Point2D.h"

#include <vector>
#include <thread>
#include <atomic>

enum SolvingAlgorithm
{
	RANDOM,
	GREEDY
};

class TravelingSalesmanSolver
{
	std::vector<Point2D> m_points;
	std::vector<Point2D*> m_route;

	unsigned int m_timeStepNanoseconds = 0;
	SolvingAlgorithm m_selectedAlgorithm = RANDOM;

	std::thread m_solving;
	std::atomic<bool> m_isSolving = false;
	std::atomic<bool> m_isInterrupt = false;

public:
	TravelingSalesmanSolver(int numberOfPoints, int xMin, int xMax, int yMin, int yMax);
	~TravelingSalesmanSolver();

	bool isSolving() const { return m_isSolving; };

	const std::vector<Point2D>& getItems() const { return m_points; };
	int getNumberOfPoints() const { return m_points.size(); };

	const std::vector<Point2D*>& getRoute() const { return m_route; };
	
	unsigned int getTimeStep() const { return m_timeStepNanoseconds; };
	void setTimeStep(int timeStepNanoseconds) { m_timeStepNanoseconds = timeStepNanoseconds; };

	const SolvingAlgorithm getSelectedAlgorithm() { return m_selectedAlgorithm; };
	void setSolvingAlgorithm(SolvingAlgorithm solvingAlgorithm) { m_selectedAlgorithm = solvingAlgorithm; };

	void generatePoints(int numberOfPoints, int xMin, int xMax, int yMin, int yMax);

public:
	void greedyAlgorithm();


};

