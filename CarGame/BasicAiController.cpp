#include "stdafx.h"
#include "BasicAIController.h"
#include "Car.h"
#include <iostream>
#include <iomanip>
#include "Car.h"
#include "fann.h"
#include "Application.h"
#include "Track.h"
#include "Box2DDebugDraw.h"

BasicAIController::BasicAIController()
{

}

void BasicAIController::initController(TopdownCar * car)
{
	IController::initController(car);

	//Default values for a good AI
	m_params[EBASICAI_MAXSPEED]					= 1.0f;
	m_params[EBASICAI_ANGLETOTURN]				= 10.0f;
	m_params[EBASICAI_DISTANCETOFRONTWALL_STOP] = 0.05f;
	m_params[EBASICAI_LOOKAHEAD_DISTANCE]		= 5.0f;
	m_params[EBASICAI_ANGLETOTURNSPEEDINFLUENCE]= 1.f / 90.0f;
}

void BasicAIController::keyEvent(unsigned char c, bool keypress)
{
}

BasicAIController::~BasicAIController()
{

}

void BasicAIController::setParams(float maxspeed, float angletoturn, float distancetofrontwallstop, float lookahead, float angletoturnspeedinfluence)
{
	m_params[EBASICAI_MAXSPEED]					= maxspeed;
	m_params[EBASICAI_ANGLETOTURN]				= angletoturn;
	m_params[EBASICAI_DISTANCETOFRONTWALL_STOP] = distancetofrontwallstop;
	m_params[EBASICAI_LOOKAHEAD_DISTANCE]		= lookahead;
	m_params[EBASICAI_ANGLETOTURNSPEEDINFLUENCE] = angletoturnspeedinfluence;
}

void BasicAIController::fixedStepUpdate()
{

	m_currentAction[OA_DOWN] = m_currentAction[OA_LEFT] = m_currentAction[OA_RIGHT] = m_currentAction[OA_UP] = -1.0f;
	
	float *sensorData = m_car->getSensorData()->data;

	SGenTrackNode &toSector = TRACK->getSectorPoint(m_car->getCurrentRaceSectorIdx() + floor(m_params[EBASICAI_LOOKAHEAD_DISTANCE]));
	b2Vec2 diff = toSector.racingPoint - m_car->getPosition();
	b2Vec2 dir = diff;
	b2Vec2 forwardCar = m_car->getCarModel()->getDirection();
	dir.Normalize();
	
	float dot	= -forwardCar.x * dir.y + forwardCar.y * dir.x;
	float angleInDegrees		= glm::degrees(acos(dot));
	float targetAngleInDegrees  = 90;// front of car
	float diffAngleToTarget		= targetAngleInDegrees - angleInDegrees;

	if (diffAngleToTarget > m_params[EBASICAI_ANGLETOTURN])
		m_currentAction[OA_RIGHT] = 1.0f;
	else
	if (diffAngleToTarget < m_params[EBASICAI_ANGLETOTURN])
		m_currentAction[OA_LEFT] = 1.0f;

	float maxSpeedPercent = m_params[EBASICAI_MAXSPEED] * (1.f - diffAngleToTarget * m_params[EBASICAI_ANGLETOTURNSPEEDINFLUENCE]);
	
	if(sensorData[IS_VELOCITY] < maxSpeedPercent && sensorData[IS_RAYCAST0] > m_params[EBASICAI_DISTANCETOFRONTWALL_STOP])
		m_currentAction[OA_UP] = 1.0f; 

	m_car->setAction(m_currentAction);

	DebugLine l;
	l.p1 = m_car->getPosition();
	l.p2 = toSector.racingPoint;

	RENDER->ddraw->debug_lines.push_back(l);

	
}