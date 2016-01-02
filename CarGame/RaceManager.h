#pragma once
#include "IController.h"

class TopdownCar;

class RaceManager : public b2ContactListener
{
protected:
	IController   ** m_controllers;
	unsigned int	m_numRacers;
	unsigned int	m_numLaps;
	unsigned int    m_currentNumLaps;

	float			m_maxRaceTime;
	unsigned int	m_startRaceTick;
	unsigned int	m_currentRaceTicks;

	bool			m_isRaceEnded;

public:
	RaceManager();
	~RaceManager();

	virtual void init();

	void keyEvent(const char key, bool pressed);
	void loadRaceFromTOML(const char *filename);

	virtual void updateControllers();
	virtual void updateModels();

	void BeginContact(b2Contact* contact) { handleContact(contact, true); }
	void EndContact(b2Contact* contact) { handleContact(contact, false); }

	void handleContact(b2Contact* contact, bool began);
	void carVsGroundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began);

	TopdownCar * getCarFromID(unsigned int id);

	void debugDraw();


};