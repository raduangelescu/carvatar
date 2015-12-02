#pragma once
#include "stdafx.h"
#include "Car.h"
#include "Track.h"
#include "Box2DDebugDraw.h"

extern PhysicsData g_physicsInfo;
extern CTrack*		g_trackInfo;
extern Box2DDebugDraw* g_debugDraw;

TopdownCar::TopdownCar()
{

	g_physicsInfo.world->SetGravity(b2Vec2(0, 0));
	m_car = new TDCar(g_physicsInfo.world);


	//m_tire = new TDTire(m_world);
	//m_tire->setCharacteristics(100, -20, 150);
	


	m_controlState = 0;
}

void TopdownCar::setPosition(b2Vec2 pos)
{
	m_car->setPosition(pos);
	m_car->setTransform(pos, -1.5707f);
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
		m_controlState |= TDC_LEFT; 
		break;
	case 'd': 
		m_controlState |= TDC_RIGHT; 
		break;
	case 'w': 
		m_controlState |= TDC_UP; 
		break;
	case 's': 
		m_controlState |= TDC_DOWN; 
		break;
	}
}

void TopdownCar::keyboardUp(unsigned char key)
{
	switch (key) 
	{
	case 'a': 
		m_controlState &= ~TDC_LEFT;
		break;
	case 'd': 
		m_controlState &= ~TDC_RIGHT; 
		break;
	case 'w': 
		m_controlState &= ~TDC_UP; 
		break;
	case 's': 
		m_controlState &= ~TDC_DOWN; 
		break;
	}
}

void TopdownCar::handleContact(b2Contact* contact, bool began)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();
	FixtureUserData* fudA = (FixtureUserData*)a->GetUserData();
	FixtureUserData* fudB = (FixtureUserData*)b->GetUserData();

	if (!fudA || !fudB)
		return;


	if (fudA->getType() == FUD_CAR_TIRE || fudB->getType() == FUD_GROUND_AREA)
		tire_vs_groundArea(a, b, began);
	else if (fudA->getType() == FUD_GROUND_AREA || fudB->getType() == FUD_CAR_TIRE)
		tire_vs_groundArea(b, a, began);

}

void TopdownCar::tire_vs_groundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began)
{
	TDTire* tire = (TDTire*)tireFixture->GetBody()->GetUserData();
	RaceSectorFUD* gaFud = (RaceSectorFUD*)groundAreaFixture->GetUserData();

	if (gaFud->idx == m_currentRaceSectorIdx + 1)
		m_currentRaceSectorIdx++;
	//printf("RACE SECTOR: %d  you have %f race done \n", m_currentRaceSectorIdx, (float)m_currentRaceSectorIdx / (g_trackInfo->getFinishLineRaceSectorIdx()));
	
	if (m_currentRaceSectorIdx == (g_trackInfo->getFinishLineRaceSectorIdx() ))
	{
		printf("RACE LAP FINISHED \n");
		m_currentRaceSectorIdx = 0;
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
	for (b2Body* b = g_physicsInfo.world->GetBodyList(); b; b = b->GetNext()) {
		if (b == m_car->getBody() || b->GetType() != b2_staticBody)
			continue;
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
			if (f->IsSensor() == true)
				continue;
			b2RayCastOutput output;
			
			if (!f->RayCast(&output, input,0))
				continue;
			if (output.fraction < closestFraction) {
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

void RotateVector(b2Vec2 vec, float angle, b2Vec2 &output)
{
	float cs = cos(angle);
	float sn = sin(angle);

	output.x = vec.x * cs - vec.y * sn;
	output.y = vec.x * sn + vec.y * cs;

}

void TopdownCar::step()
{

	m_car->update(m_controlState);
	//normalized
//	m_sensorData.data[IS_VELOCITY]= m_car->getBody()->GetLinearVelocity().Length()/60.0f;
	
	SGenTrackNode& sector =  g_trackInfo->getTrackPoint(m_currentRaceSectorIdx);

	//normalized
	static float maxDistance = g_trackInfo->getDistanceToFinishLine(0);
//	m_sensorData.data[IS_LEFTDISTANCE] = g_trackInfo->getDistanceToFinishLine(m_currentRaceSectorIdx)/ maxDistance; //+ (sector.center - m_car->getBody()->GetPosition()).Length();
	
	b2Vec2 trackdirection = sector.direction;
	b2Vec2 cardirection = m_car->getDirection();
	//normalized?
//	m_sensorData.data[IS_TRACKANGLE] = (cardirection.x * trackdirection.x + cardirection.y * trackdirection.y + 1.0f)/2.0f;
	b2RayCastInput input;
	
	float rayLength = 10000; //long enough to hit the walls
	b2Vec2 front = m_car->getDirection();
	b2Vec2 right, left,hright,hleft;
	RotateVector(front, 90 * DEGTORAD, right);
	RotateVector(front, -90 * DEGTORAD, left);
	RotateVector(front, 45 * DEGTORAD, hright);
	RotateVector(front, -45 * DEGTORAD, hleft);
	float scaleRaycastData = 100;
//	m_sensorData.data[IS_RAYCAST90] = Raycast(right, rayLength) * scaleRaycastData;
	m_sensorData.data[IS_RAYCAST45] = Raycast(hright, rayLength) * scaleRaycastData * 10.0f;
//	m_sensorData.data[IS_RAYCAST0] = Raycast(front, rayLength) * scaleRaycastData;
	m_sensorData.data[IS_RAYCASTM45] = Raycast(hleft, rayLength) * scaleRaycastData * 10.0f;
//	m_sensorData.data[IS_RAYCASTM90] = Raycast(left, rayLength) * scaleRaycastData;
	Raycast(hleft, rayLength);

	

	
}

