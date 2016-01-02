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
#include "Application.h"

RaceManager::RaceManager()
{
	m_startRaceTick = 0;
	m_currentRaceTicks = 0;
	m_isRaceEnded = false;
}

RaceManager::~RaceManager()
{
	for (unsigned int i = 0; i < m_numRacers; i++)
		delete m_controllers[i];
	delete[] m_controllers;
}

void RaceManager::init()
{
	loadRaceFromTOML("racesetup.TOML");
}

void RaceManager::loadRaceFromTOML(const char *filename)
{
	std::ifstream ifs(filename);
	toml::Parser parser(ifs);
	toml::Value documentRoot = parser.parse();
	toml::Value* raceSettings = documentRoot.find("RaceSettings");
	const toml::Array& racers = raceSettings->find("racers")->as<toml::Array>();

	m_numRacers = racers.size();
	m_controllers = new IController*[m_numRacers];

	unsigned int i = 0;
	for (const toml::Value& racer : racers)
	{
		// To be used :)
		std::string racer_name = racer.find("name")->as<std::string>();
		
		std::string racer_type = racer.find("type")->as<std::string>();
		const toml::Value *color = racer.find("color");
	
		TopdownCar * car = new TopdownCar(i, b2Color(color->find("r")->as<double>(), color->find("g")->as<double>(), color->find("b")->as<double>()));

		car->setPosition(TRACK->getSectorPoint(0).center);

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

		// get Params if any
		const toml::Value * params = racer.find("params");
		if (params)
		{
			const toml::Array vparams = params->as<toml::Array>();
			((BasicAIController*)m_controllers[i])->setParams(
				vparams.at(0).as<double>(),
				vparams.at(1).as<double>(),
				vparams.at(2).as<double>(),
				vparams.at(3).as<double>(),
				vparams.at(4).as<double>());
		}

		i++;
	}
	
	m_numLaps		= raceSettings->find("lap_number")->as<int>();
	m_maxRaceTime	= raceSettings->find("max_race_time")->as<int>();
	
}

void RaceManager::keyEvent(const char key, bool pressed)
{
	for (unsigned int i = 0; i < m_numRacers; i++)
		m_controllers[i]->keyEvent(key,pressed);
}

void RaceManager::updateModels()
{

	bool shouldEndRace = true;
	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		if (!m_controllers[i]->getCar()->getHasFinishedRace())
		{
			shouldEndRace = false;
		}
		
		m_controllers[i]->getCar()->step();
		
	}
	if (shouldEndRace)
		m_isRaceEnded = true;
	if (m_currentRaceTicks >= m_maxRaceTime)
	{
		m_isRaceEnded = true;
	}
}

void RaceManager::updateControllers()
{
	if (m_isRaceEnded)
		return;

	m_currentRaceTicks++;

	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		if (!m_controllers[i]->getCar()->getHasFinishedRace())
			m_controllers[i]->fixedStepUpdate();
	}
}	

void RaceManager::handleContact(b2Contact* contact, bool began)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();
	FixtureUserData* fudA = (FixtureUserData*)a->GetUserData();
	FixtureUserData* fudB = (FixtureUserData*)b->GetUserData();

	if (a->GetFilterData().categoryBits == CATEGORY_STATIC && fudA == NULL)
	{
		if (fudB != NULL && fudB->getType() == FUD_CAR)
		{
			TopdownCar* car = getCarFromID(((CarFUD*)fudB)->getId());
			if (car != NULL)
			{
				car->getFitnessData()->data[FT_NUMCRASHES]++;
			}
		}
	}
	if (b->GetFilterData().categoryBits == CATEGORY_STATIC && fudB == NULL)
	{
		if (fudA != NULL && fudA->getType() == FUD_CAR)
		{
			TopdownCar* car = getCarFromID(((CarFUD*)fudA)->getId());
			if (car != NULL)
			{
				car->getFitnessData()->data[FT_NUMCRASHES]++;
			}
		}
	}

	if (!fudA || !fudB)
		return;


	if (fudA->getType() == FUD_CAR || fudB->getType() == FUD_RACE_SECTOR)
		carVsGroundArea(a, b, began);
	else if (fudA->getType() == FUD_RACE_SECTOR || fudB->getType() == FUD_CAR)
		carVsGroundArea(b, a, began);
}

TopdownCar * RaceManager::getCarFromID(unsigned int id)
{
	TopdownCar * car = NULL;
	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		car = m_controllers[i]->getCar();
		car = (car->getCarModel()->getId() != id) ? NULL : car;
		if (car != NULL)
			return car;
	}
	return NULL;
}

void RaceManager::carVsGroundArea(b2Fixture* carFixture, b2Fixture* groundAreaFixture, bool began)
{
	CarFUD*			cFUD  = (CarFUD*)carFixture->GetUserData();
	RaceSectorFUD*  gFUD  = (RaceSectorFUD*)groundAreaFixture->GetUserData();
	
	TopdownCar * car = NULL;
	car = getCarFromID(cFUD->getId());

	if (car != NULL)
	{
		unsigned int raceSector = car->getCurrentRaceSectorIdx();
		if (gFUD->idx == raceSector + 1)
			raceSector++;
		//m_currentRaceSectorIdx = gaFud->idx;
		//printf("RACE SECTOR: %d  you have %f race done \n", m_currentRaceSectorIdx, (float)m_currentRaceSectorIdx / (TRACK->getFinishLineRaceSectorIdx()));

		if (raceSector == (TRACK->getFinishLineRaceSectorIdx()))
		{
			raceSector = 0;
			unsigned int newLap = car->getCurrentLap() + 1;
			car->setCurrentLap(newLap);
			if (newLap > m_numLaps)
			{
				car->setHasFinishedRace(true);
			}
			car->getFitnessData()->data[FT_INVERSELAPTIME] = car->getFitnessData()->data[FT_INVERSELAPTIME] + 1.0f/m_currentRaceTicks;
			printf("RACE LAP FINISHED \n");
		}

		car->setCurrentRaceSectorIdx(raceSector);
	}
}

void RaceManager::debugDraw()
{
	for (unsigned int i = 0; i < m_numRacers; i++)
	{
		m_controllers[i]->getCar()->debugDraw();
	}
}