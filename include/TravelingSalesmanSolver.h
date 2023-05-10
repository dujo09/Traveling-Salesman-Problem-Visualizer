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
	float m_xMin, m_xMax, m_yMin, m_yMax;

	std::vector<Point2D> m_points;
	std::vector<Point2D*> m_route;

	unsigned int m_timeStepNanoseconds = 100;
	SolvingAlgorithm m_selectedAlgorithm = RANDOM;

	std::thread m_solving;
	std::atomic<bool> m_isSolving = false;
	std::atomic<bool> m_isInterrupt = false;

public:
	TravelingSalesmanSolver(int numberOfPoints, float xMin, float xMax, float yMin, float yMax);
	~TravelingSalesmanSolver();

	bool isSolving() const { return m_isSolving; };

	bool isInterrupt() const { return m_isInterrupt; };
	void interruptSolving() { m_isInterrupt = true; };

	const std::vector<Point2D>& getPoints() const { return m_points; };
	const std::vector<Point2D*>& getRoute() const { return m_route; };
	
	unsigned int getTimeStep() const { return m_timeStepNanoseconds; };
	void setTimeStep(int timeStepNanoseconds) { m_timeStepNanoseconds = timeStepNanoseconds; };

	SolvingAlgorithm getSelectedAlgorithm() { return m_selectedAlgorithm; };
	void setSolvingAlgorithm(SolvingAlgorithm solvingAlgorithm) { m_selectedAlgorithm = solvingAlgorithm; };

	float getXMin() { return m_xMin; };
	void setXMin(float xMin) { m_xMin = xMin; };
	float getXMax() { return m_xMax; };
	void setXMax(float xMax) { m_xMax = xMax; };
	float getYMin() { return m_yMin; };
	void setYMin(float yMin) { m_yMin = yMin; };
	float getYMax() { return m_yMax; };
	void setYMax(float yMax) { m_yMax = yMax; };

	void generatePoints(int numberOfPoints);

	void startSolving();

private:
	void _startSolving();
	void greedyAlgorithm();


};

