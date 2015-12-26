#pragma once
#include "stdafx.h"
#include "CarModel.h"
#include "Track.h"
#include <tinytoml.h>

extern PhysicsData	g_physicsInfo;
extern CTrack*		g_trackInfo;

CarFUD::CarFUD(unsigned int id) : FixtureUserData(FUD_CAR),m_id(id)
{
}

b2Vec2 CarModel::getLateralVelocity()
{
	b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 CarModel::getForwardVelocity()
{
	b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, m_body->GetLinearVelocity()) * currentForwardNormal;
}


void CarModel::updateDrive(int controlState)
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

	m_body->ApplyForce(m_currentTraction * force * currentForwardNormal , m_body->GetWorldCenter(), true);
}

void CarModel::updateTurn(int controlState)
{
	b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
	float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);
	if (fabs(currentSpeed) < 0.1f)
		return;
	float desiredTorque = 0;
	switch (controlState & (IA_LEFT | IA_RIGHT)) 
	{
		case IA_LEFT:  desiredTorque = 15 * 0.001f;  break;
		case IA_RIGHT: desiredTorque = -15 * 0.001f; break;
		default:;//nothing
	}
	
	m_body->ApplyTorque(desiredTorque, true);
}

b2Vec2  CarModel::getDirection()
{
	return m_body->GetWorldVector(b2Vec2(0, 1));
}


CarModel::CarModel(b2World* world,unsigned int id)
{
	m_id = id;
	std::ifstream ifs("carsetup.TOML");
	toml::Parser parser(ifs);
	
	toml::Value documentRoot = parser.parse();
	toml::Value* params = documentRoot.find("carparams");

	m_maxForwardSpeed = (float)params->find("max_forward_speed")->as<double>();
	m_maxBackwardSpeed = (float)params->find("max_backward_speed")->as<double>();
	m_maxDriveForce = (float)params->find("max_drive_force")->as<double>();
	m_maxLateralImpulse = (float)params->find("max_lateral_impulse")->as<double>();
	m_angularDamping = (float)params->find("angular_damping")->as<double>();
	m_bodyDensity = (float)params->find("body_density")->as<double>();
	m_currentTraction = (float)params->find("traction")->as<double>();

	
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
	m_body->SetAngularDamping(m_angularDamping);

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, bodyVertices.size());
	b2Fixture* fixture = m_body->CreateFixture(&polygonShape, m_bodyDensity);

	b2Filter filter;
	filter.categoryBits = CATEGORY_CAR;
	filter.maskBits		= CATEGORY_STATIC;
	
	fixture->SetFilterData(filter);
	fixture->SetUserData(new CarFUD(id));

	delete[] vertices;

}

void CarModel::setTransform(b2Vec2 position, float angle)
{
	m_body->SetTransform(position,angle);
}

CarModel::~CarModel()
{
}

void CarModel::updateFriction() 
{
	b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
	if (impulse.Length() > m_maxLateralImpulse)
		impulse *= m_maxLateralImpulse / impulse.Length();

	m_body->ApplyLinearImpulse(0.05 * impulse, m_body->GetWorldCenter(), true);

	//angular velocity
	m_body->ApplyAngularImpulse(m_currentTraction *0.23 * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	m_body->ApplyForce(m_currentTraction* 0.23 * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}


void CarModel::update(int controlState)
{
	updateFriction();
	updateDrive(controlState);
	updateTurn(controlState);
}

void CarModel::setPosition(b2Vec2 position)
{
	m_body->SetActive(false);
	m_body->SetAwake(false);
	m_body->SetTransform(position, 0.0f);
	

	m_body->SetActive(true);
	m_body->SetAwake(true);
	
}

