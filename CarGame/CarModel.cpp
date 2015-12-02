#pragma once
#include "stdafx.h"
#include "CarModel.h"
#include "Track.h"

extern PhysicsData	g_physicsInfo;
extern CTrack*		g_trackInfo;

CarTireFUD::CarTireFUD() : FixtureUserData(FUD_CAR_TIRE)
{
}

TDTire::TDTire(b2World* world) 
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.5f, 1.25f);
	b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 1);//shape, density
	CarTireFUD * ctFud = new CarTireFUD();
	fixture->SetUserData(ctFud);

	m_body->SetUserData(this);

	m_currentTraction = 1;
}

TDTire::~TDTire() 
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void TDTire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse) 
{
	m_maxForwardSpeed = maxForwardSpeed;
	m_maxBackwardSpeed = maxBackwardSpeed;
	m_maxDriveForce = maxDriveForce;
	m_maxLateralImpulse = maxLateralImpulse;
}

void TDTire::updateTraction()
{
	std::set<GroundAreaFUD*> & groudnAreas = g_trackInfo->getGroundAreas(); 
	if (groudnAreas.empty())
		m_currentTraction = 1;
	else {
		//find area with highest traction
		m_currentTraction = 0;
		std::set<GroundAreaFUD*>::iterator it = groudnAreas.begin();
		while (it != groudnAreas.end()) {
			GroundAreaFUD* ga = *it;
			if (ga->frictionModifier > m_currentTraction)
				m_currentTraction = ga->frictionModifier;
			++it;
		}
	}
}

b2Vec2 TDTire::getLateralVelocity() 
{
	b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 TDTire::getForwardVelocity() 
{
	b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, m_body->GetLinearVelocity()) * currentForwardNormal;
}

void TDTire::updateFriction() 
{
	//lateral linear velocity
	b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
	if (impulse.Length() > m_maxLateralImpulse)
		impulse *= m_maxLateralImpulse / impulse.Length();
	m_body->ApplyLinearImpulse(m_currentTraction * impulse, m_body->GetWorldCenter(), true);

	//angular velocity
	m_body->ApplyAngularImpulse(m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	m_body->ApplyForce(m_currentTraction * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void TDTire::updateDrive(int controlState) 
{
	m_body->SetAwake(true);
	//find desired speed
	float desiredSpeed = 0;
	switch (controlState & (TDC_UP | TDC_DOWN)) {
	case TDC_UP:   desiredSpeed = m_maxForwardSpeed;  break;
	case TDC_DOWN: desiredSpeed = m_maxBackwardSpeed; break;
	default: return;//do nothing
	}

	//find current speed in forward direction
	b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
	float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

	//apply necessary force
	float force = 0;
	if (desiredSpeed > currentSpeed)
		force = m_maxDriveForce;
	else if (desiredSpeed < currentSpeed)
		force = -m_maxDriveForce;
	else
		return;
	m_body->ApplyForce(m_currentTraction * force * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void TDTire::updateTurn(int controlState)
{
	float desiredTorque = 0;
	switch (controlState & (TDC_LEFT | TDC_RIGHT)) 
	{
		case TDC_LEFT:  desiredTorque = 15;  break;
		case TDC_RIGHT: desiredTorque = -15; break;
		default:;//nothing
	}
	
	m_body->ApplyTorque(desiredTorque, true);
}

b2Vec2  TDCar::getDirection()
{

	float cs = m_body->GetTransform().q.c;
	float sn = m_body->GetTransform().q.s;
	b2Vec2 frontw;
	frontw.Set(0.0f, 1.0f);
	b2Vec2 front;

	front.x = frontw.x * cs - frontw.y * sn;
	front.y = frontw.x * sn + frontw.y * cs;

	return front;
}

TDCar::TDCar(b2World* world) 
{

	//create car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	m_body = world->CreateBody(&bodyDef);
	m_body->SetAngularDamping(3);

	b2Vec2 vertices[8];
	vertices[0].Set(1.5f, 0);
	vertices[1].Set(3.0f, 2.5f);
	vertices[2].Set(2.8f, 5.5f);
	vertices[3].Set(1.0f, 10.0f);
	vertices[4].Set(-1.0f, 10.0f);
	vertices[5].Set(-2.8f, 5.5f);
	vertices[6].Set(-3.0f, 2.5f);
	vertices[7].Set(-1.0f, 0.0f);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);
	b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.1f);//shape, density

																	//prepare common joint parameters
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = m_body;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;
	jointDef.upperAngle = 0;
	jointDef.localAnchorB.SetZero();//center of tire

	float maxForwardSpeed;
	float maxBackwardSpeed;
	float backTireMaxDriveForce;
	float frontTireMaxDriveForce;
	float backTireMaxLateralImpulse;
	float frontTireMaxLateralImpulse;

	FILE *carconfigFile = fopen("carsetup.ini","r");
	char dummyvar[1024];
	fscanf(carconfigFile, "%s %f", dummyvar,&maxForwardSpeed);
	fscanf(carconfigFile, "%s %f", dummyvar, &maxBackwardSpeed);
	fscanf(carconfigFile, "%s %f", dummyvar, &backTireMaxDriveForce);
	fscanf(carconfigFile, "%s %f", dummyvar, &frontTireMaxDriveForce);
	fscanf(carconfigFile, "%s %f", dummyvar, &backTireMaxLateralImpulse);
	fscanf(carconfigFile, "%s %f", dummyvar, &frontTireMaxLateralImpulse);
	fclose(carconfigFile);
	

	//back left tire
	TDTire* tire = new TDTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(-3.0f, 0.75f);
	world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//back right tire
	tire = new TDTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(3.0f, 0.75f);
	world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//front left tire
	tire = new TDTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(-3.0f, 8.5f);
	flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//front right tire
	tire = new TDTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(3.0f, 8.5f);
	frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

}
void TDCar::setTransform(b2Vec2 position, float angle)
{
	m_body->SetTransform(position,angle);
}
TDCar::~TDCar() 
{
	for (unsigned int i = 0; i < m_tires.size(); i++)
		delete m_tires[i];
}

void TDCar::update(int controlState) 
{
	for (unsigned int i = 0; i < m_tires.size(); i++)
		m_tires[i]->updateFriction();
	for (unsigned int i = 0; i < m_tires.size(); i++)
		m_tires[i]->updateDrive(controlState);
	//control steering
	float lockAngle = 35 * DEGTORAD;
	float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
	float turnPerTimeStep = turnSpeedPerSec / 60.0f;
	float desiredAngle = 0;
	bool shouldTurn = false;
	switch (controlState & (TDC_LEFT | TDC_RIGHT))
	{
	case TDC_LEFT:
	{
		desiredAngle = lockAngle;
		shouldTurn = true;
		break;
	}
	case TDC_RIGHT:
	{
		shouldTurn = true;
		desiredAngle = -lockAngle; break;
	}
	default:
		shouldTurn = false;
		flJoint->SetLimits(desiredAngle, desiredAngle);
		frJoint->SetLimits(desiredAngle, desiredAngle);
		break;
		//nothing
	}
	if (shouldTurn)
	{ 

		float angleNow = flJoint->GetJointAngle();
		float angleToTurn = desiredAngle - angleNow;
		angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
		float newAngle = angleNow + angleToTurn;
		flJoint->SetLimits(newAngle, newAngle);
		frJoint->SetLimits(newAngle, newAngle);
	}
}

void TDCar::setPosition(b2Vec2 position)
{
	m_body->SetActive(false);
	m_body->SetAwake(false);
	m_body->SetTransform(position, 0.0f);
	for (unsigned int i = 0; i < m_tires.size(); i++)
	{
		m_tires[i]->m_body->SetActive(false);
		m_tires[i]->m_body->SetActive(false);
		m_tires[i]->m_body->SetTransform(position, 0.0f);
	}

	m_body->SetActive(true);
	m_body->SetAwake(true);
	for (unsigned int i = 0; i < m_tires.size(); i++)
	{
		m_tires[i]->m_body->SetActive(true);
		m_tires[i]->m_body->SetActive(true);
	}

}

