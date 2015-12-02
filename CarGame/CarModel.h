#pragma once

#include "stdafx.h"
#include "Common.h"


//class to allow marking a fixture as a car tire
class CarTireFUD : public FixtureUserData 
{
public:
	CarTireFUD();
};




class TDTire
{

public:
	
	b2Body* m_body;
	float m_maxForwardSpeed;
	float m_maxBackwardSpeed;
	float m_maxDriveForce;
	float m_maxLateralImpulse;

	float m_currentTraction;

	TDTire(b2World* world);
	~TDTire();

	void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse);

	void updateTraction();

	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();
	void updateFriction();

	void updateDrive(int controlState);
	void updateTurn(int controlState);
};


class TDCar 
{
	b2Body* m_body;
	std::vector<TDTire*> m_tires;
	b2RevoluteJoint *flJoint, *frJoint;

public:

	TDCar(b2World* world);
	~TDCar();

	void update(int controlState);
	b2Body*	getBody() { return m_body; }
	b2Vec2  getDirection();
	void setTransform(b2Vec2 position, float angle);

	public:
		void setPosition(b2Vec2 position);

};
