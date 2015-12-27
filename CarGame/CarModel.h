#pragma once

#include "stdafx.h"
#include "Common.h"


class CarFUD : public FixtureUserData
{
	unsigned int m_id;
public:
	CarFUD(unsigned int id);
	~CarFUD();

	unsigned int getId() { return m_id; }
};


class CarModel 
{
	b2Body* m_body;

	float m_maxForwardSpeed;
	float m_maxBackwardSpeed;
	float m_maxDriveForce;
	float m_maxLateralImpulse;
	float m_currentTraction;
	float m_angularDamping;
	float m_bodyDensity;

	unsigned int m_id;

public:

	CarModel(b2World* world, unsigned int id);
	~CarModel();

	void update(float* controlState);
	b2Body*	getBody() { return m_body; }
	b2Vec2  getDirection();
	float getMaxFrontSpeed() { return m_maxForwardSpeed; }
	void setTransform(b2Vec2 position, float angle);
	
	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();

	void   updateDrive(float* controlState);
	void   updateTurn(float* controlState);
	void   updateFriction();
	
	unsigned int getId() { return m_id; }
	
	void setPosition(b2Vec2 position);

};
