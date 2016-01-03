#pragma once
#include "RaceManager.h"

class TopdownCar;
class BasicAIController;

enum DE_STATE
{
	DE_CREATECHILDREN,
	DE_RUNSIMULATION,
	DE_EVOLVE,
	DE_END,

	DE_NUMSTATES
};

class DifferentialEvolution : public RaceManager
{
	IController   ** m_childrenControllers;
	BasicAIController * m_bestCandidate;
	DE_STATE m_currentState;

	int m_maxIterations;
	int m_populationSize;
	float m_differentialWeight;
	float m_crossoverProbability;
	
	void loadRaceFromTOML(const char *filename);
	int pickUnique(int *blackList, int size);	
	void stepDifferential();
	void evolve();

public:

	DifferentialEvolution();
	~DifferentialEvolution();

	virtual void init();

	virtual void updateControllers();


};