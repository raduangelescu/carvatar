#pragma once

#include "Common.h"

//class to allow marking a fixture as a ground area
class RaceSectorFUD : public FixtureUserData 
{
public:
	int idx;
	RaceSectorFUD(int _idx);

};


struct SGenTrackNode
{
	b2Vec2	inner;
	b2Vec2	outer;
	b2Vec2  center;
	b2Vec2  racingPoint;
	b2Vec2	direction;
	float	width;
};

class CTrack
{
	b2Color	m_debugColorSectors;
	b2Color m_debugColorWall;
	//Track settings
	int		m_trackSize;
	int		m_downStep;
	int		m_sectorStep;
	int		m_trackWidth;
	int		m_radiusCurvature[2];
	int		m_radiusOffsetCurvature;
	int		m_hardCurvatureProbability[3];
	int		m_hardCurvatureValue;
	int		m_smoothIterations;
	float	m_physicsWallSizeInner[2];
	float	m_physicsWallSizeOuter[2];
	//

	unsigned int m_allPointsSize;
	unsigned int m_finishLineRaceSectorIdx;
	float		 m_trackLength;

	SGenTrackNode			*m_points;
	b2Body					*m_groundBody;

private:
	void			lerpInterval(int startIdx, int endIdx, b2Vec2* points, int size);
	void			genCenterline(b2Vec2 *points);
	void			genLogicalTrackRepresentation(b2Vec2 *points);

	void			genRacingLineApproximation();

public:
	CTrack();
	~CTrack();
	void			loadSettingsFromTOML(const char *filename);
	void			genTrack();
	void			genPhysicsTrackRepresentation();

	float			getDistanceToFinishLine(const unsigned int idx);
	float			getSectorDistanceToCenterline(const unsigned int idx, b2Vec2 carPos);
	float			getTotalTrackLength() { return m_trackLength; }
	SGenTrackNode & getSectorPoint(const unsigned int idx);
	unsigned int	getFinishLineRaceSectorIdx() { return m_finishLineRaceSectorIdx; }
	b2Body*			getGroundBody() { return m_groundBody; }

	void			debugDraw();
	
	void destroyPhysicsBody();
	void clear();
	
};