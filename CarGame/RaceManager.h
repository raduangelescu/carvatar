#pragma once
#include "IController.h"

class RaceManager : public b2ContactListener
{
private:
	IController   ** m_controllers;
	unsigned int	m_numRacers;
public:
	RaceManager();
	~RaceManager();

	void loadRaceFromTOML(const char *filename);

	void Update();
	void BeginContact(b2Contact* contact) { handleContact(contact, true); }
	void EndContact(b2Contact* contact) { handleContact(contact, false); }

	void handleContact(b2Contact* contact, bool began);
	void carVsGroundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began);


};