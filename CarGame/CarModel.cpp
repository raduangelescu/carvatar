#pragma once
#include "stdafx.h"
#include "CarModel.h"
#include "Track.h"
#include <tinytoml.h>

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
	m_currentTraction = 1;
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
	switch (controlState & (IA_UP | IA_DOWN)) 
	{
		case IA_UP:   
			desiredSpeed = m_maxForwardSpeed;  
			break;
		case IA_DOWN: 
			desiredSpeed = m_maxBackwardSpeed; 
			break;
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
	switch (controlState & (IA_LEFT | IA_RIGHT)) 
	{
		case IA_LEFT:  desiredTorque = 15;  break;
		case IA_RIGHT: desiredTorque = -15; break;
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
	std::ifstream ifs("carsetup.TOML");
	toml::Parser parser(ifs);
	
	toml::Value documentRoot = parser.parse();
	toml::Value* params = documentRoot.find("carparams");

	float maxForwardSpeed			= (float)params->find("max_forward_speed")->as<double>();
	float maxBackwardSpeed			= (float)params->find("max_backward_speed")->as<double>();
	float backTireMaxDriveForce		= (float)params->find("back_tire_max_drive_force")->as<double>();
	float frontTireMaxDriveForce	= (float)params->find("front_tire_max_drive_force")->as<double>();
	float backTireMaxLateralImpulse = (float)params->find("back_tire_max_lateral_impulse")->as<double>();
	float frontTireMaxLateralImpulse= (float)params->find("front_tire_max_lateral_impulse")->as<double>();
	float angularDamping			= (float)params->find("angular_damping")->as<double>();
	float bodyDensity				= (float)params->find("body_density")->as<double>();

	
	toml::Value* carBodyParams = documentRoot.find("carbody");

	const toml::Array& bodyVertices = carBodyParams->find("body_vertices")->as<toml::Array>();
	
	b2Vec2 *vertices = new b2Vec2[bodyVertices.size()];
	unsigned int k = 0;
	for (const toml::Value& v : bodyVertices)
	{
		const toml::Array& bodyVerticesCoords = v.as<toml::Array>();
		vertices[k++].Set(bodyVerticesCoords.at(0).asNumber(), bodyVerticesCoords.at(1).asNumber());
	}

	//create car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	m_body = world->CreateBody(&bodyDef);
	m_body->SetAngularDamping(angularDamping);

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, bodyVertices.size());
	b2Fixture* fixture = m_body->CreateFixture(&polygonShape, bodyDensity);
	delete[] vertices;

	const toml::Array& joints = carBodyParams->find("joints")->as<toml::Array>();
	for (const toml::Value& v : joints)
	{
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = m_body;
		jointDef.enableLimit = true;
		jointDef.lowerAngle = 0;
		jointDef.upperAngle = 0;
		jointDef.localAnchorB.SetZero();

		const toml::Array& pos = v.find("local_anchorA")->as<toml::Array>();
		jointDef.localAnchorA.Set(pos.at(0).asNumber(), pos.at(1).asNumber());
	
		TDTire* tire = new TDTire(world);
		m_tires.push_back(tire);

		tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
		jointDef.bodyB = tire->m_body;
		b2RevoluteJoint * newJoint = (b2RevoluteJoint *)world->CreateJoint(&jointDef);
		if (v.find("frontleft"))
		{
			flJoint = newJoint;
		}
		else
		if (v.find("frontright"))
		{
			frJoint = newJoint;
		}
		
	}

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
	float lockAngle = glm::radians(35.0f);
	float turnSpeedPerSec = glm::radians(160.0f);//from lock to lock in 0.5 sec
	float turnPerTimeStep = turnSpeedPerSec / 60.0f;
	float desiredAngle = 0;
	bool shouldTurn = false;
	switch (controlState & (IA_LEFT | IA_RIGHT))
	{
	case IA_LEFT:
	{
		desiredAngle = lockAngle;
		shouldTurn = true;
		break;
	}
	case IA_RIGHT:
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

