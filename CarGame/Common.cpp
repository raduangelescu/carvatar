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
	if (min == max)
		return min;
	int offset = 0;
	if (min < 0)
	{
		max = max - min;
		offset = min;
		min = 0;

	}

	return (float)(rand() % max + min) + offset;
}

b2Vec2 Lerp(b2Vec2 start, b2Vec2 end, float alpha)
{
	b2Vec2 dir = end - start;
	float length = dir.Normalize();

	return start + alpha * length * dir;
}
