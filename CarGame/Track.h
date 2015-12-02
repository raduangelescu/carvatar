#pragma once

#include "Common.h"

extern PhysicsData g_physicsInfo;

//class to allow marking a fixture as a ground area
class RaceSectorFUD : public FixtureUserData 
{
public:
	int idx;
	RaceSectorFUD(int _idx);

};

//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData 
{
public:
	float frictionModifier;
	bool outOfCourse;

	GroundAreaFUD(float fm, bool ooc);
};

struct SGenTrackNode
{
	b2Vec2	inner;
	b2Vec2	outer;
	b2Vec2  center;
	b2Vec2	direction;
	float	width;
};

class CTrack
{
	unsigned int m_allPointsSize;
	unsigned int m_downStep;
	unsigned int m_finishLineRaceSectorIdx;
	unsigned int m_sectorStep;

	float		 m_trackLength;

	std::set<GroundAreaFUD*> m_groundAreas;
	SGenTrackNode			*m_points;
	b2Body					*m_groundBody;


public:
	CTrack();
	~CTrack();
	
	void			genTrack(unsigned int size = 360 , unsigned int downStep = 10);
	float			getDistanceToFinishLine(const unsigned int idx);
	float			getTotalTrackLength() { return m_trackLength; }
	SGenTrackNode & getTrackPoint(const unsigned int idx);
	unsigned int	getFinishLineRaceSectorIdx() { return m_finishLineRaceSectorIdx; }
	b2Body*			getGroundBody() { return m_groundBody; }
	std::set<GroundAreaFUD*> & getGroundAreas() { return m_groundAreas; }
	void clear();
	
};