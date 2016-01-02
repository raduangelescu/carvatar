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
	float m_F;

	void initGenome();
	void initPopulation();

	int pickUnique(int *blackList, int size);
	void crossover(TopdownCar *a, TopdownCar *b, TopdownCar *c);

	void stepDifferential();
	void evolve();

public:

	DifferentialEvolution();
	~DifferentialEvolution();

	virtual void init();

	virtual void updateControllers();
	virtual void updateModels();


};