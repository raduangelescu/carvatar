#pragma once
#include "stdafx.h"
#include "RaceManager.h"
#include "stdafx.h"
#include "Car.h"
#include "Track.h"
#include "Box2DDebugDraw.h"
#include "PlayerController.h"
#include "AINNController.h"
#include "BasicAIController.h"
#include <tinytoml.h>

extern PhysicsData g_physicsInfo;
extern CTrack*		g_trackInfo;


RaceManager::RaceManager()
{

}

RaceManager::~RaceManager()
{
	for (unsigned int i = 0; i < m_numRacers; i++)
		delete m_controllers[i];
}

void RaceManager::loadRaceFromTOML(const char *filename)
{
	std::ifstream ifs(filename);
	toml::Parser parser(ifs);
	toml::Value documentRoot = parser.parse();
	toml::Value* raceSettings = documentRoot.find("RaceSettings");
	const toml::Array& racers = raceSettings->find("racers")->as<toml::Array>();

	unsigned int i = 0;
	for (const toml::Value& racer : racers)
	{
		std::string racer_name = racer.find("name")->as<std::string>();
		std::string racer_type = racer.find("type")->as<std::string>();

		TopdownCar * car = new TopdownCar(i);

		car->setPosition(g_trackInfo->getTrackPoint(0).center);

		if (racer_type.compare("basic_ai") == 0)
		{
			m_controllers[i] = new BasicAIController();
		}
		else if (racer_type.compare("player") == 0)
		{
			m_controllers[i] = new PlayerController();
		}

		// link controller to car;
		m_controllers[i]->initController(car);
		i++;
	}

	
}

void RaceManager::Update()
{
	for (unsigned int i = 0; i < m_numRacers; i++)
		m_controllers[i]->fixedStepUpdate();
}	

void RaceManager::handleContact(b2Contact* contact, bool began)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();
	FixtureUserData* fudA = (FixtureUserData*)a->GetUserData();
	FixtureUserData* fudB = (FixtureUserData*)b->GetUserData();

	if (!fudA || !fudB)
		return;


	if (fudA->getType() == FUD_CAR || fudB->getType() == FUD_RACE_SECTOR)
		carVsGroundArea(a, b, began);
	else if (fudA->getType() == FUD_RACE_SECTOR || fudB->getType() == FUD_CAR)
		carVsGroundArea(b, a, began);
}


void RaceManager::carVsGroundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began)
{
	CarFUD *cFUD = (CarFUD*)tireFixture->GetUserData();
	RaceSectorFUD* gaFud = (RaceSectorFUD*)groundAreaFixture->GetUserData();
	TopdownCar * car = NULL;
	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		if (m_controllers[i]->m_car->getID() == cFUD->getId())
		{
			car = m_controllers[i]->m_car;
		}
	}

	if (car == NULL)
		return;


	if (gaFud->idx == car->m_currentRaceSectorIdx + 1)
		car->m_currentRaceSectorIdx++;

	//m_currentRaceSectorIdx = gaFud->idx;
	//printf("RACE SECTOR: %d  you have %f race done \n", m_currentRaceSectorIdx, (float)m_currentRaceSectorIdx / (g_trackInfo->getFinishLineRaceSectorIdx()));

	if (car->m_currentRaceSectorIdx == (g_trackInfo->getFinishLineRaceSectorIdx()))
	{
		printf("RACE LAP FINISHED \n");
		car->m_currentRaceSectorIdx = 0;
	}
}