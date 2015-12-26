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

	struct STopDownCarSettings
	{
		float max_forward_speed;
		float max_backward_speed;
		float back_tire_max_drive_force;
		float front_tire_max_drive_force;
		float back_tire_max_lateral_impulse;
		float front_tire_max_lateral_impulse;
		float body_density;
		
	}m_settings; // read from TOML file

	CarSensorData	m_sensorData;
private:
	float Raycast(b2Vec2 dir, float distance);
public:
	TopdownCar(unsigned int id);

	~TopdownCar();

	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);
	
	
	
	void step();

	b2Vec2 getPosition();
	void setPosition(b2Vec2 pos);
	CarSensorData & getSensorData() { return m_sensorData; }

	b2Body* m_groundBody;
	CarModel* m_car;
	
	int m_controlState;
	unsigned int m_currentRaceSectorIdx;
};
