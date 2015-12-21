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

class TopdownCar : public b2ContactListener
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
	TopdownCar();

	~TopdownCar();

	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);
	
	void handleContact(b2Contact* contact, bool began);

	void BeginContact(b2Contact* contact) { handleContact(contact, true); }
	void EndContact(b2Contact* contact) { handleContact(contact, false); }

	void tireVsGroundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began);
	void step();

	b2Vec2 getPosition();
	void setPosition(b2Vec2 pos);
	CarSensorData & getSensorData() { return m_sensorData; }

	b2Body* m_groundBody;
	TDCar* m_car;
	
	int m_controlState;
	unsigned int m_currentRaceSectorIdx;

};
