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

void BasicAIController::setParams(float *params)
{
	memcpy(m_params, params, sizeof(m_params));
}

void BasicAIController::fixedStepUpdate()
{
	// Reset our current action
	m_currentAction[OA_DOWN] = m_currentAction[OA_LEFT] = m_currentAction[OA_RIGHT] = m_currentAction[OA_UP] = -1.0f;
	
	// Get our sensor data
	float *sensorData = m_car->getSensorData()->data;

	//Get the lookat race line point
	SGenTrackNode &toSector = TRACK->getSectorPoint(m_car->getCurrentRaceSectorIdx() + floor(m_params[EBASICAI_LOOKAHEAD_DISTANCE]));
	//Create the target vector
	b2Vec2 diff = toSector.racingPoint - m_car->getPosition();
	//Create the direction to target vector
	b2Vec2 dir = diff;
	dir.Normalize();
	
	// Get the forward car vector
	b2Vec2 forwardCar = m_car->getCarModel()->getDirection();
	
	// Get the angle between the car front and the direction to target
	// map it to degree space 
	float dot	= -forwardCar.x * dir.y + forwardCar.y * dir.x;
	float angleInDegrees		= glm::degrees(acos(dot));
	float targetAngleInDegrees  = 90;// front of car
	float diffAngleToTarget		= targetAngleInDegrees - angleInDegrees;

	// Turn the car in the direction that minimizes the angle between the car forward vector
	// and the to target vector
	if (diffAngleToTarget > m_params[EBASICAI_ANGLETOTURN])
		m_currentAction[OA_RIGHT] = 1.0f;
	else
	if (diffAngleToTarget < m_params[EBASICAI_ANGLETOTURN])
		m_currentAction[OA_LEFT] = 1.0f;

	// Tune the speed procent we are safe to accelerate within
	// note that it is invers proportional to the to target angle (or you can perceive this as curvature)
	float maxSpeedPercent = m_params[EBASICAI_MAXSPEED] * (1.f - diffAngleToTarget * m_params[EBASICAI_ANGLETOTURNSPEEDINFLUENCE]);
	
	// Accelerate if we are within safe velocity limits and if we did not hit a wall
	if(sensorData[IS_VELOCITY] < maxSpeedPercent && sensorData[IS_RAYCAST0] > m_params[EBASICAI_DISTANCETOFRONTWALL_STOP])
		m_currentAction[OA_UP] = 1.0f; 

	//Send the action
	m_car->setAction(m_currentAction);

	//Draw the debug line to the current target point we are following
	/*DebugLine l;
	l.p1 = m_car->getPosition();
	l.p2 = toSector.racingPoint;

	RENDER->ddraw->debug_lines.push_back(l);*/

	
}