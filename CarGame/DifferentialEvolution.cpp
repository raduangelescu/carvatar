#include "stdafx.h"
#include "DifferentialEvolution.h"
#include "BasicAIController.h"
#include "Car.h"
#include "Application.h"
#include "Track.h"

DifferentialEvolution::DifferentialEvolution(): RaceManager()
{

}

DifferentialEvolution::~DifferentialEvolution()
{

}

void DifferentialEvolution::initGenome()
{

}

void DifferentialEvolution::initPopulation()
{

}


void DifferentialEvolution::crossover(TopdownCar *a, TopdownCar *b, TopdownCar *c)
{

}


void DifferentialEvolution::init()
{
	m_currentState = DE_CREATECHILDREN;
	m_numLaps = 1000;
	m_maxIterations = 10;
	m_populationSize = 50;
	m_maxRaceTime = 500;
	m_numRacers = m_populationSize;
	m_controllers = new IController*[m_populationSize];
	m_childrenControllers = new IController*[m_populationSize];
	m_bestCandidate = NULL;
	m_F = 1.0f;
	m_crossoverProbability = 0.5f;


	unsigned int i = 0;
	for (; i < m_populationSize; i++)
	{
		
		TopdownCar * car = new TopdownCar(i, b2Color(randomInterval(0, 255), randomInterval(0, 255), randomInterval(0, 255)));
		car->setPosition(TRACK->getSectorPoint(0).center);
		m_controllers[i] = new BasicAIController();
		m_controllers[i]->initController(car);
		
		float paramsInit[EBASICAI_NUM];
		
		// random uniform parameters
		for (unsigned int j = 0; j < EBASICAI_NUM; j++)
		{
			paramsInit[j] = randomInterval(0, 10000) / 10000.0f;
		}

		paramsInit[EBASICAI_ANGLETOTURN] = randomInterval(0, 60);
		paramsInit[EBASICAI_LOOKAHEAD_DISTANCE] = randomInterval(2, 20);
		paramsInit[EBASICAI_ANGLETOTURNSPEEDINFLUENCE] = 1.0f/randomInterval(1, 10000);
		paramsInit[EBASICAI_MAXSPEED] = randomInterval(1, 1000)/1000.0f;
		paramsInit[EBASICAI_DISTANCETOFRONTWALL_STOP] = 1.0f / randomInterval(1, 1000);
		//init random
		((BasicAIController*)m_controllers[i])->setParams(paramsInit);
	}

	for (i = 0; i < m_populationSize; i++)
	{
		TopdownCar * car = new TopdownCar(i + m_populationSize, b2Color(randomInterval(0, 255), randomInterval(0, 255), randomInterval(0, 255)));
		car->setPosition(TRACK->getSectorPoint(0).center);
		m_childrenControllers[i] = new BasicAIController();
		m_childrenControllers[i]->initController(car);
	}
}

int DifferentialEvolution::pickUnique(int *blackList, int size)
{
	bool isNotUnique = false;
	int newIdx = -1;
	do
	{
		isNotUnique = false;
		newIdx = floor(rand() % (m_populationSize - 1));
		for (unsigned int i = 0; i < size; i++)
		{
			if (newIdx == blackList[i])
			{
				isNotUnique = true;
				break;
			}
		}
	} while (isNotUnique);
	
	return newIdx;
}

void DifferentialEvolution::evolve()
{
	float bestCurrentFitness = -1000000000;
	if (m_bestCandidate != NULL)
	{
		bestCurrentFitness = m_bestCandidate->getCar()->getFitnessData()->ComputeFitness();
		m_bestCandidate->getCar()->setDebugColor(b2Color(255, 255, 255));
	}
	
	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		float parentFitness = ((BasicAIController*)m_controllers[i])->getCar()->getFitnessData()->ComputeFitness();
		float childFitness = ((BasicAIController*)m_childrenControllers[i])->getCar()->getFitnessData()->ComputeFitness();

		if (parentFitness < childFitness)
		{
			((BasicAIController*)m_controllers[i])->setParams(((BasicAIController*)m_childrenControllers[i])->getParams());
		}
		if (bestCurrentFitness < parentFitness || bestCurrentFitness < childFitness)
		{
			m_bestCandidate = (BasicAIController*)m_controllers[i];
		}
	}
	if (m_bestCandidate != NULL)
		m_bestCandidate->getCar()->setDebugColor(b2Color(255, 0, 0));

}

void DifferentialEvolution::stepDifferential()
{
	for (unsigned int i = 0; i < m_populationSize; i++)
	{
		int pickIdx[3];
		
		pickIdx[0] = pickUnique(pickIdx, 0);
		pickIdx[1] = pickUnique(pickIdx, 1);
		pickIdx[2] = pickUnique(pickIdx, 2);
		// pick random R
		int R = randomInterval(0, EBASICAI_NUM);
		
		// Crossover
		BasicAIController * individualA = (BasicAIController*)m_controllers[pickIdx[0]];
		BasicAIController * individualB = (BasicAIController*)m_controllers[pickIdx[1]];
		BasicAIController * individualC = (BasicAIController*)m_controllers[pickIdx[2]];

		float newParams[EBASICAI_NUM];

		for (unsigned int j = 0; j < EBASICAI_NUM; j++)
		{
			
			if(randomInterval(0, EBASICAI_NUM) == R | rand() % 1< m_crossoverProbability)
			{
				newParams[j] = individualA->getParams()[j] + m_F * (individualB->getParams()[j] - individualC->getParams()[j]);
			}
			else
			{
				newParams[j] = individualA->getParams()[j];
			}
			
		}
		((BasicAIController*)m_childrenControllers[i])->setParams(newParams);

	}

}




void DifferentialEvolution::updateControllers()
{
	switch (m_currentState)
	{
		case DE_CREATECHILDREN:
		{
			m_maxIterations--;
			stepDifferential();

			if (m_maxIterations > 0)
			{
				m_currentState = DE_RUNSIMULATION;
				m_isRaceEnded = false;
				m_currentRaceTicks = 0;
			}
			else
			{
				m_currentState = DE_END;
				printf("Best candidate params are: ");
				prinfvector(m_bestCandidate->getParams(), EBASICAI_NUM);
				printf("Best candidate params are: ");
			}
			break;
		}
		case DE_RUNSIMULATION:
		{
			RaceManager::updateControllers();

			for (unsigned int i = 0; i < m_numRacers; i++)
			{
				m_childrenControllers[i]->fixedStepUpdate();
			}

			float bestCurrentFitness = -1000000000;
			if (m_bestCandidate != NULL)
			{
				bestCurrentFitness = m_bestCandidate->getCar()->getFitnessData()->ComputeFitness();
				m_bestCandidate->getCar()->setDebugColor(b2Color(255, 255, 255));
			}

			for (unsigned int i = 0; i < m_populationSize; i++)
			{
				float parentFitness = ((BasicAIController*)m_controllers[i])->getCar()->getFitnessData()->ComputeFitness();
				float childFitness = ((BasicAIController*)m_childrenControllers[i])->getCar()->getFitnessData()->ComputeFitness();

			
				if (bestCurrentFitness < parentFitness)
				{
					m_bestCandidate = (BasicAIController*)m_controllers[i];
				}
				
				if (bestCurrentFitness < childFitness)
				{
					m_bestCandidate = (BasicAIController*)m_childrenControllers[i];
				}
			}
			if (m_bestCandidate != NULL)
				m_bestCandidate->getCar()->setDebugColor(b2Color(255, 0, 0));

			if (m_isRaceEnded == true)
			{
				m_currentState = DE_EVOLVE;
			}
			break;
		}
		case DE_EVOLVE:
		{
			evolve();
			unsigned int i = 0;
			//Reset race
			for (; i < m_populationSize; i++)
			{
				TopdownCar * car = m_controllers[i]->getCar();
				car->setPosition(TRACK->getSectorPoint(0).center);
				car->reset();
			}
			m_currentState = DE_CREATECHILDREN;
			break;
		}
		case DE_END:
		{
	
			// DO NOTHING
			break;
		}
	}

}

void DifferentialEvolution::updateModels()
{
	RaceManager::updateModels();
	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		m_childrenControllers[i]->getCar()->step();
	}
}