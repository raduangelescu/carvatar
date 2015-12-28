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

class TopdownCar 
{
	b2Color			m_debugColor;
	CarSensorData	m_sensorData;
	
public:

	TopdownCar(unsigned int id,b2Color color);
	~TopdownCar();

	void step();

	void setAction(float *currentAction);

	b2Vec2 getPosition();
	void   setPosition(b2Vec2 pos);
	
	CarModel*      getCarModel()  { return m_car; }
	CarSensorData* getSensorData(){ return &m_sensorData; }

	unsigned int   getCurrentRaceSectorIdx()				{ return m_currentRaceSectorIdx; }
	void		   setCurrentRaceSectorIdx(unsigned int s) { m_currentRaceSectorIdx = s; }

	void debugDraw();

private:
	CarModel*    m_car;
	float        m_controlState[OA_NUM];
	unsigned int m_currentRaceSectorIdx;



};
