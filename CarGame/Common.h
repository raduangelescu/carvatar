#pragma once
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "defines.h"


enum INPUT_ACTIONS 
{
	TDC_LEFT = 0x1,
	TDC_RIGHT = 0x2,
	TDC_UP = 0x4,
	TDC_DOWN = 0x8
};



//types of fixture user data
enum fixtureUserDataType 
{
	FUD_CAR_TIRE,
	FUD_GROUND_AREA
};

//a class to allow subclassing of different fixture user data
class FixtureUserData 
{
	fixtureUserDataType m_type;
protected:
	FixtureUserData(fixtureUserDataType type);
public:
	virtual fixtureUserDataType getType();
	virtual ~FixtureUserData();
};

float randomInterval(int min, int max);
b2Vec2 Lerp(b2Vec2 start, b2Vec2 end, float alpha);