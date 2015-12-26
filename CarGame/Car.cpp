#pragma once
#include "stdafx.h"
#include "Car.h"
#include "Track.h"
#include "Box2DDebugDraw.h"

extern PhysicsData g_physicsInfo;
extern CTrack*		g_trackInfo;
extern Box2DDebugDraw* g_debugDraw;

TopdownCar::TopdownCar(unsigned int id)
{
	m_car					= new CarModel(g_physicsInfo.world,id);
	m_currentRaceSectorIdx	= 0;
	m_controlState			= 0;

	g_physicsInfo.world->SetGravity(b2Vec2(0, 0));
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

void TopdownCar::keyboard(unsigned char key)
{
	switch (key)
	{
	case 'a': 
		m_controlState |= IA_LEFT; 
		break;
	case 'd': 
		m_controlState |= IA_RIGHT; 
		break;
	case 'w': 
		m_controlState |= IA_UP; 
		break;
	case 's': 
		m_controlState |= IA_DOWN; 
		break;
	}
}

void TopdownCar::keyboardUp(unsigned char key)
{
	switch (key) 
	{
	case 'a': 
		m_controlState &= ~IA_LEFT;
		break;
	case 'd': 
		m_controlState &= ~IA_RIGHT; 
		break;
	case 'w': 
		m_controlState &= ~IA_UP; 
		break;
	case 's': 
		m_controlState &= ~IA_DOWN; 
		break;
	}
}



b2Vec2 TopdownCar::getPosition()
{
	return m_car->getBody()->GetPosition();
}

float TopdownCar::Raycast(b2Vec2 dir, float distance)
{
	b2RayCastInput input;
	
	b2Vec2 p1 = m_car->getBody()->GetPosition();
	b2Vec2 p2 = p1 + distance * dir;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b = g_physicsInfo.world->GetBodyList(); b; b = b->GetNext()) 
	{
		if (b == m_car->getBody() || b->GetType() != b2_staticBody)
			continue;
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) 
		{
			if (f->IsSensor() == true)
				continue;
			b2RayCastOutput output;
			
			if (!f->RayCast(&output, input,0))
				continue;
			if (output.fraction < closestFraction) 
			{
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	DebugLine l;
	l.p1 = p1;
	l.p2 = p1 + distance * closestFraction * dir;

	g_debugDraw->debug_lines.push_back(l);

	return closestFraction;
}



void TopdownCar::step()
{

	m_car->update(m_controlState);
	//normalized
	m_sensorData.data[IS_VELOCITY]= m_car->getBody()->GetLinearVelocity().Length()/m_car->getMaxFrontSpeed();
	
	SGenTrackNode& sector =  g_trackInfo->getTrackPoint(m_currentRaceSectorIdx);

	//normalized
	static float maxDistance = g_trackInfo->getDistanceToFinishLine(0);
 	m_sensorData.data[IS_LEFTDISTANCE] = g_trackInfo->getDistanceToFinishLine(m_currentRaceSectorIdx)/ maxDistance; 
	
	b2Vec2 trackdirection = sector.direction;
	b2Vec2 cardirection = m_car->getDirection();
	//normalized?
	m_sensorData.data[IS_TRACKANGLE]			  = (cardirection.x * trackdirection.x + cardirection.y * trackdirection.y + 1.0f)/2.0f;

	m_sensorData.data[IS_CARDISTANCETOCENTERLINE] = g_trackInfo->getSectorDistanceToCenterline(m_currentRaceSectorIdx + 1, m_car->getBody()->GetPosition());
	b2RayCastInput input;
	
	float rayLength = 100; //long enough to hit the walls
	b2Vec2 front = m_car->getDirection();
	b2Vec2 right, left,hright,hleft;
	RotateVector(front, 90 * DEGTORAD, right);
	RotateVector(front, -90 * DEGTORAD, left);
	RotateVector(front, 45 * DEGTORAD, hright);
	RotateVector(front, -45 * DEGTORAD, hleft);

	m_sensorData.data[IS_RAYCAST90] = Raycast(right, rayLength);
	m_sensorData.data[IS_RAYCAST45] = Raycast(hright, rayLength);
	m_sensorData.data[IS_RAYCAST0] = Raycast(front, rayLength);
	m_sensorData.data[IS_RAYCASTM45] = Raycast(hleft, rayLength);
	m_sensorData.data[IS_RAYCASTM90] = Raycast(left, rayLength);

	
	Raycast(hleft, rayLength);

	

	
}

