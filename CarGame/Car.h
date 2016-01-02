#pragma once

#include "CarModel.h"

struct CarSensorData
{
	float data[IS_NUM];

	CarSensorData()
	{
		memset(data, 0, sizeof(data));
	}
};

struct FitnessData
{
	float data[FT_NUM];
	
	FitnessData()
	{
		memset(data, 0, sizeof(data));
	}

	float ComputeFitness()
	{
		float empiricalWeights[FT_NUM];
		empiricalWeights[FT_NUMCRASHES]		= -0.01f;
		empiricalWeights[FT_INVERSELAPTIME] = 1000.0f;
		empiricalWeights[FT_DISTANCELEFT]   = 100.0f;
		
		float fitness = 0;
		// dot product
		for (unsigned int i = 0; i < FT_NUM; i++)
		{
			fitness += empiricalWeights[i] * data[i];
		}
		return fitness;
	}
};

class TopdownCar 
{
	b2Color			m_debugColor;
	CarSensorData	m_sensorData;
	FitnessData		m_fitnessData;
	CarModel*		m_car;
	float			m_controlState[OA_NUM];
	unsigned int	m_currentRaceSectorIdx;
	unsigned int	m_currentLap;

	bool			m_hasFinishedRace;

public:

	TopdownCar(unsigned int id,b2Color color);
	~TopdownCar();

	void step();
	void reset();
	void setAction(float *currentAction);

	b2Vec2 getPosition();
	void   setPosition(b2Vec2 pos);
	
	CarModel*      getCarModel()  { return m_car; }
	CarSensorData* getSensorData(){ return &m_sensorData; }
	FitnessData*   getFitnessData() { return &m_fitnessData; }

	bool		   getHasFinishedRace() {return m_hasFinishedRace;}
	void		   setHasFinishedRace(bool val) { m_hasFinishedRace = val; }

	unsigned int   getCurrentLap() {return m_currentLap;}
	void		   setCurrentLap(unsigned int val) { m_currentLap = val; }


	unsigned int   getCurrentRaceSectorIdx()				{ return m_currentRaceSectorIdx; }
	void		   setCurrentRaceSectorIdx(unsigned int s) { m_currentRaceSectorIdx = s; }

	void setDebugColor(b2Color	&color){ m_debugColor = color; }
	void debugDraw();

};
