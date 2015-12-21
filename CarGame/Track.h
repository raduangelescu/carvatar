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
	struct STrackGenSettings
	{
		int track_size;
		int down_step;
		int sector_step;
		int track_width;
		int radius_curvature[2];
		int radius_offset_curvature;
		int hard_curvature_probability[3];
		int hard_curvature_value;
		int	smooth_iterations;
		int physics_wall_size_inner[2];
		int physics_wall_size_outer[2];

	}m_settings; // read from TOML file

	unsigned int m_allPointsSize;
	unsigned int m_downStep;
	unsigned int m_finishLineRaceSectorIdx;
	unsigned int m_sectorStep;

	float		 m_trackLength;

	std::set<GroundAreaFUD*> m_groundAreas;
	SGenTrackNode			*m_points;
	b2Body					*m_groundBody;

private:
	void			lerpInterval(int startIdx, int endIdx, b2Vec2* points, int size);

public:
	CTrack();
	~CTrack();
	void			loadSettingsFromTOML(const char *filename);
	void			genTrack();
	void			genCenterline(b2Vec2 *points);
	void			genLogicalTrackRepresentation(b2Vec2 *points);
	void			genPhysicsTrackRepresentation();
	float			getDistanceToFinishLine(const unsigned int idx);
	float			getTotalTrackLength() { return m_trackLength; }
	SGenTrackNode & getTrackPoint(const unsigned int idx);
	unsigned int	getFinishLineRaceSectorIdx() { return m_finishLineRaceSectorIdx; }
	b2Body*			getGroundBody() { return m_groundBody; }
	std::set<GroundAreaFUD*> & getGroundAreas() { return m_groundAreas; }
	void destroyGround();
	void clear();
	
};