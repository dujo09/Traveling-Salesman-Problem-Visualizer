#pragma once

#include "Point2D.h"

#include <vector>
#include <thread>
#include <atomic>

enum SolvingAlgorithm
{
	GREEDY,
	TWO_OPT,

};

class TravelingSalesmanSolver
{
	std::vector<Point2D> m_points;
	std::vector<Point2D*> m_route;

	unsigned int m_timeStepMilliseconds = 1000;
	SolvingAlgorithm m_selectedAlgorithm = TWO_OPT;

	std::thread m_solving;
	std::atomic<bool> m_isSolving = false;
	std::atomic<bool> m_isInterrupt = false;

	float m_routeLength = 0;

public:
	static int MAX_TIME_STEP_MILLISECONDS;

public:
	TravelingSalesmanSolver(int numberOfPoints, float xMin, float xMax, float yMin, float yMax);
	~TravelingSalesmanSolver();

	bool isSolving() const { return m_isSolving; };

	bool isInterrupt() const { return m_isInterrupt; };
	void interruptSolving() { m_isInterrupt = true; };

	const std::vector<Point2D>& getPoints() const { return m_points; };
	int getNumberOfPoints() const { return m_points.size(); };
	const std::vector<Point2D*>& getRoute() const { return m_route; };
	
	unsigned int getTimeStep() const { return m_timeStepMilliseconds; };
	void setTimeStep(int timeStepMilliseconds) 
	{ 
		if (timeStepMilliseconds < 0)
		{
			return;
		}
		m_timeStepMilliseconds = timeStepMilliseconds; 
	};

	SolvingAlgorithm getSelectedAlgorithm() { return m_selectedAlgorithm; };
	void setSolvingAlgorithm(SolvingAlgorithm solvingAlgorithm) 
	{ 
		if (m_isSolving)
		{
			return;
		}

		m_selectedAlgorithm = solvingAlgorithm; 
	};

	float getRouteLength() const { return m_routeLength; };

	void generatePoints(int numberOfPoints, float xMin, float xMax, float yMin, float yMax);

	void startSolving();

private:
	void _startSolving();
	void setAllPointsColors(glm::vec3 color);
	void setWholeRouteColor(glm::vec3 color);

};

