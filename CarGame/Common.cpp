#include "stdafx.h"
#include "Common.h"

FixtureUserData::FixtureUserData(fixtureUserDataType type) : m_type(type)
{
}

fixtureUserDataType FixtureUserData::getType()
{ 
	return m_type; 
}

FixtureUserData::~FixtureUserData() 
{
}

float randomInterval(int min, int max)
{
	return (float)(rand() % max + min);
}

b2Vec2 Lerp(b2Vec2 start, b2Vec2 end, float alpha)
{
	b2Vec2 dir = end - start;
	dir.Normalize();

	return start + alpha * (end - start).Length() * dir;
}
