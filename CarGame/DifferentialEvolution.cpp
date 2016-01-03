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

void DifferentialEvolution::init()
{
	loadRaceFromTOML("racesetup.TOML");

	m_currentState = DE_CREATECHILDREN;
	
	m_numRacers = m_populationSize * 2;
	m_controllers = new IController*[m_numRacers];
	m_childrenControllers = &m_controllers[m_populationSize];
	m_bestCandidate = NULL;
	m_crossoverProbability = 0.5f;

	unsigned int i = 0;
	for (; i < m_numRacers; i++)
	{
		
		TopdownCar * car = new TopdownCar(i, b2Color(0.0f, randomInterval(128, 255) / 255.0f, randomInterval(128, 255) / 255.0f));
		car->setPosition(TRACK->getSectorPoint(0).center);
		m_controllers[i] = new BasicAIController();
		m_controllers[i]->initController(car);

		if (i <= m_populationSize)
		{
			//parents
			float paramsInit[EBASICAI_NUM];

			paramsInit[EBASICAI_ANGLETOTURN] = randomInterval(0, 60);
			paramsInit[EBASICAI_LOOKAHEAD_DISTANCE] = randomInterval(2, 20);
			paramsInit[EBASICAI_ANGLETOTURNSPEEDINFLUENCE] = 1.0f / randomInterval(1, 10000);
			paramsInit[EBASICAI_MAXSPEED] = randomInterval(0, 110) / 100.0f;
			paramsInit[EBASICAI_DISTANCETOFRONTWALL_STOP] = 1.0f / randomInterval(1, 1000);
			//init random
			((BasicAIController*)m_controllers[i])->setParams(paramsInit);
		}
		else
		{
			m_controllers[i]->getCar()->setDebugColor( b2Color(0.5f, 0.5, 0.5f));
			//children
		}
	}

}


void DifferentialEvolution::loadRaceFromTOML(const char *filename)
{
	std::ifstream ifs(filename);
	toml::Parser parser(ifs);
	toml::Value documentRoot = parser.parse();
	toml::Value* raceSettings = documentRoot.find("DifferentialEvolution");

	m_numLaps = raceSettings->find("lap_number")->as<int>();
	m_maxRaceTime = raceSettings->find("max_race_time")->as<int>();
	m_populationSize = raceSettings->find("population_size")->as<int>();
	m_maxIterations = raceSettings->find("max_iterations")->as<int>();
	m_differentialWeight = raceSettings->find("differential_weight")->as<double>();
	m_crossoverProbability = raceSettings->find("crossover_probability")->as<double>();
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
		
		BasicAIController * x = (BasicAIController*)m_controllers[i];
		// Crossover
		BasicAIController * individualA = (BasicAIController*)m_controllers[pickIdx[0]];
		BasicAIController * individualB = (BasicAIController*)m_controllers[pickIdx[1]];
		BasicAIController * individualC = (BasicAIController*)m_controllers[pickIdx[2]];

		float newParams[EBASICAI_NUM];
		
		memcpy(newParams, x->getParams(), sizeof(newParams));

		for (unsigned int j = 0; j < EBASICAI_NUM; j++)
		{
			float pickRi = randomInterval(0,10000)/10000.0f;
			
			if(pickRi < m_crossoverProbability || j == R)
			{
				newParams[j] = individualA->getParams()[j] + m_differentialWeight * (individualB->getParams()[j] - individualC->getParams()[j]);
			}
			
		}
		((BasicAIController*)m_childrenControllers[i])->setParams(newParams);
		((BasicAIController*)m_childrenControllers[i])->getCar()->setDebugColor(b2Color(0.f, 0.f, 0.f));
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
			for (; i < m_numRacers; i++)
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
