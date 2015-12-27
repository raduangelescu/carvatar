#pragma once
#include "stdafx.h"
#include "Car.h"
#include "Track.h"
#include "Box2DDebugDraw.h"
#include "Application.h"

TopdownCar::TopdownCar(unsigned int id)
{
	m_car					= new CarModel(PHYSX->world,id);
	m_currentRaceSectorIdx	= 0;

	PHYSX->world->SetGravity(b2Vec2(0, 0));

}

void TopdownCar::setPosition(b2Vec2 pos)
{
	m_car->setPosition(pos);
	m_car->setTransform(pos, glm::radians(-90.0));
}

TopdownCar::~TopdownCar()
{
	delete m_car;
}

void TopdownCar::setAction(float *currentAction)
{
	memcpy(m_controlState, currentAction, sizeof(m_controlState));
}



b2Vec2 TopdownCar::getPosition()
{
	return m_car->getBody()->GetPosition();
}

float Raycast(b2Vec2 startPosition, b2Vec2 dir, float distance)
{
	b2RayCastInput input;
	
	b2Vec2 p1 = startPosition;
	b2Vec2 p2 = p1 + distance * dir;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);

	for (b2Body* b = PHYSX->world->GetBodyList(); b; b = b->GetNext()) 
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) 
		{
			if (f->GetFilterData().categoryBits == CATEGORY_STATIC && !f->IsSensor())
			{
				b2RayCastOutput output;

				if (!f->RayCast(&output, input, 0))
					continue;
				
				if (output.fraction < closestFraction)
				{
					closestFraction    = output.fraction;
					intersectionNormal = output.normal;
				}
			}
		}
	}
	// Add raycast to debug renderer
	DebugLine l;
	l.p1 = p1;
	l.p2 = p1 + distance * closestFraction * dir;

	RENDER->ddraw->debug_lines.push_back(l);

	return closestFraction;
}



void TopdownCar::step()
{

	m_car->update(m_controlState);
	//normalized
	m_sensorData.data[IS_VELOCITY]= m_car->getBody()->GetLinearVelocity().Length()/m_car->getMaxFrontSpeed();
	
	SGenTrackNode& sector =  TRACK->getTrackPoint(m_currentRaceSectorIdx);

	//normalized
	static float maxDistance = TRACK->getDistanceToFinishLine(0);
 	m_sensorData.data[IS_LEFTDISTANCE] = TRACK->getDistanceToFinishLine(m_currentRaceSectorIdx)/ maxDistance; 
	
	b2Vec2 trackdirection = sector.direction;
	b2Vec2 cardirection = m_car->getDirection();
	//normalized?
	m_sensorData.data[IS_TRACKANGLE]			  = (cardirection.x * trackdirection.x + cardirection.y * trackdirection.y + 1.0f)/2.0f;

	m_sensorData.data[IS_CARDISTANCETOCENTERLINE] = TRACK->getSectorDistanceToCenterline(m_currentRaceSectorIdx + 1, m_car->getBody()->GetPosition());
	b2RayCastInput input;
	
	float rayLength = 100; //long enough to hit the walls
	b2Vec2 front = m_car->getDirection();
	b2Vec2 pos   = m_car->getBody()->GetPosition();
	
	b2Vec2 right, left,hright,hleft;

	RotateVector(front, glm::radians(90.0f), right);
	RotateVector(front, glm::radians(-90.0f), left);
	RotateVector(front, glm::radians(45.0f), hright);
	RotateVector(front, glm::radians(-45.0f), hleft);

	m_sensorData.data[IS_RAYCAST90]  = Raycast(pos,right, rayLength);
	m_sensorData.data[IS_RAYCAST45]  = Raycast(pos,hright, rayLength);
	m_sensorData.data[IS_RAYCAST0]	 = Raycast(pos,front, rayLength);
	m_sensorData.data[IS_RAYCASTM45] = Raycast(pos,hleft, rayLength);
	m_sensorData.data[IS_RAYCASTM90] = Raycast(pos,left, rayLength);
	
}

