#include "stdafx.h"
#include "Track.h"
#include <tinytoml.h>

extern PhysicsData g_physicsInfo;

RaceSectorFUD::RaceSectorFUD(int _idx) : FixtureUserData(FUD_GROUND_AREA)
{
	idx = _idx;
}

GroundAreaFUD::GroundAreaFUD(float fm, bool ooc) : FixtureUserData(FUD_GROUND_AREA) 
{
	frictionModifier = fm;
	outOfCourse = ooc;
}

CTrack::CTrack()
{

}
void CTrack::destroyGround()
{
	m_groundBody->SetActive(false);
	g_physicsInfo.world->Step(0, 0, 0);
	g_physicsInfo.world->DestroyBody(m_groundBody);
}

void CTrack::clear()
{
	delete[] m_points;
}

CTrack::~CTrack()
{

}

SGenTrackNode & CTrack::getTrackPoint(const unsigned int idx)
{
	return m_points[idx * m_sectorStep];
}

float CTrack::getDistanceToFinishLine(const unsigned int idx)
{
	unsigned int numSectors = m_allPointsSize / m_sectorStep;
	float distance = 0.0f;
	for (unsigned int sector = idx + 1; sector < numSectors; sector++)
	{
		b2Vec2 dir = (m_points[sector].center - m_points[sector - 1].center);
		distance += dir.Length();
	}

	return distance;
}

void	CTrack::lerpInterval(int startIdx, int endIdx, b2Vec2* points, int size)
{
	const int lenInterval = endIdx - startIdx;
	for (int j = startIdx; j < endIdx && j < size; j++)
	{
		const float ammount = 1.0f - (endIdx - j) / (float)m_settings.down_step;
		points[j] = Lerp(points[startIdx], points[endIdx], ammount);
	}
}

void CTrack::genCenterline(b2Vec2 *points)
{
	
	b2Vec2 centerOfTrack = b2Vec2(SCREEN_SIZE_WIDTH / 2.0f, SCREEN_SIZE_HEIGHT / 2.0f);
	// generate first important points 
	// with a low sample rate (given by the downstep)
	int i;
	for (i = 0; i < m_settings.track_size; i += m_settings.down_step)
	{
		float randomRadius = randomInterval(m_settings.radius_curvature[0], m_settings.radius_curvature[1]);
		float randomValue =  randomInterval(m_settings.hard_curvature_probability[0], m_settings.hard_curvature_probability[1]);
		float offsetconst = (randomValue >= m_settings.hard_curvature_probability[2]) ? (float)m_settings.hard_curvature_value : (float)m_settings.radius_offset_curvature;
		float radius	  = offsetconst + randomRadius;

		float alpha = ((360.0f / (m_settings.track_size - 1)) * i) * DEGTORAD;
		// control point
		points[i] = centerOfTrack + radius * b2Vec2(sin(alpha), cos(alpha));

		// fill in the rest of the interval
		const int startIntervalIdx = i - m_settings.down_step;
		if (startIntervalIdx >= 0)
		{
			const int endIntervalIdx = i;
			lerpInterval(startIntervalIdx, endIntervalIdx, points, m_settings.track_size);
		}
	}
	//lerp last part if track_size is not a multiple of downstep
	if (i >= m_settings.track_size)
	{
		points[m_settings.track_size - 1] = points[0];
		lerpInterval(i - m_settings.down_step, m_settings.track_size-1, points, m_settings.track_size);
	}

	//apply a simple smooth filter (moving average 3, centered)
	for (int i = 0; i < m_settings.smooth_iterations; i++)
	{
		for (int k = 0; k <  m_settings.track_size; k++)
		{
			// wrap around index
			int prev_idx = ((k - 1) < 0) ? (m_settings.track_size - 1) : (k - 1);
			int next_idx = ((k + 1) >= m_settings.track_size) ? 0: (k + 1);
			
			// moving average
			points[k] = 1/3.f *(points[prev_idx] + points[k] + points[next_idx]);
		}
	}

}

void	CTrack::genPhysicsTrackRepresentation()
{
	b2BodyDef bodyDef;
	m_groundBody = g_physicsInfo.world->CreateBody(&bodyDef);
	// generate circuit phisic walls
	for (unsigned int i = 0; i < m_allPointsSize; i++)
	{

		b2PolygonShape wallShapeInner;
		wallShapeInner.SetAsBox(m_settings.physics_wall_size_inner[0], m_settings.physics_wall_size_inner[1],
								m_points[i].inner, atan2(m_points[i].direction.y, m_points[i].direction.x));

		b2FixtureDef wallfixtureDefInner;
		wallfixtureDefInner.shape = &wallShapeInner;

		b2Fixture* wallFixtureInner = m_groundBody->CreateFixture(&wallfixtureDefInner);

		b2PolygonShape wallShapeOuter;
		wallShapeOuter.SetAsBox(m_settings.physics_wall_size_outer[0], m_settings.physics_wall_size_outer[1], 
								m_points[i].outer, atan2(m_points[i].direction.y, m_points[i].direction.x));

		b2FixtureDef wallfixtureDefOuter;
		wallfixtureDefOuter.shape = &wallShapeOuter;
		b2Fixture* wallFixtureOuter = m_groundBody->CreateFixture(&wallfixtureDefOuter);

	}

	unsigned int numSectors = m_allPointsSize / m_sectorStep;

	for (unsigned int sector = 0; sector < numSectors; sector++)
	{
		unsigned int i = sector * m_sectorStep;

		b2PolygonShape groundSectorShape;
		//Generate ground sector shape
		b2Vec2 diffvec = m_points[i].inner - m_points[i].outer;
		float len = diffvec.Normalize();
		float x = diffvec.x;
		diffvec.x = -diffvec.y;
		diffvec.y = x;

		b2FixtureDef groundSectorFixtureDef;

		groundSectorShape.SetAsBox(10.0f, len / 2.0f, m_points[i].center, atan2(diffvec.y, diffvec.x));
		groundSectorFixtureDef.isSensor = true;
		groundSectorFixtureDef.shape = &groundSectorShape;

		b2Fixture* groundSectorFixture = m_groundBody->CreateFixture(&groundSectorFixtureDef);
		RaceSectorFUD *userData = new RaceSectorFUD(m_finishLineRaceSectorIdx);
		groundSectorFixture->SetUserData(userData);
		m_finishLineRaceSectorIdx++;

	}
	m_finishLineRaceSectorIdx = m_finishLineRaceSectorIdx - 1;
}


void CTrack::genLogicalTrackRepresentation(b2Vec2 *points)
{
	m_allPointsSize = m_settings.track_size;

	m_points = new SGenTrackNode[m_allPointsSize];
	float hwidth = m_settings.track_width / 2.0f;

	for (unsigned int i = 0; i < m_allPointsSize - 1; i++)
	{
		
		b2Vec2 dir = points[i + 1] - points[i];
		dir.Normalize();
		b2Vec2 ortho;
		ortho.Set(dir.y, -dir.x);

		m_points[i].center	  = points[i];
		m_points[i].direction = dir;
		m_points[i].inner	  = points[i] + hwidth * ortho;
		m_points[i].outer	  = points[i] - hwidth * ortho;
		m_points[i].width     = m_settings.track_width;
	}
	// complete the circle
	// use points[1] because [0] is the same as m_points[m_allPointsSize-1];
	b2Vec2 dirw = points[m_allPointsSize - 1] - points[1];
	dirw.Normalize();

	unsigned int endidx = m_allPointsSize - 1;
	m_points[endidx].direction = dirw;
	b2Vec2 orthow;
	orthow.Set(dirw.y, -dirw.x);
	m_points[endidx].inner  = points[endidx] + hwidth * orthow;
	m_points[endidx].outer  = points[endidx] - hwidth * orthow;
	m_points[endidx].center = points[endidx];
	m_points[endidx].width  = m_settings.track_width;
}

void CTrack::genTrack()
{
	loadSettingsFromTOML("TrackGeneration.TOML");
	m_finishLineRaceSectorIdx = 0;
	m_allPointsSize = m_settings.track_size;
	m_downStep		= m_settings.down_step;
	m_trackLength	= 0.0f;
	m_sectorStep	= m_settings.sector_step;

	b2Vec2 *centerlinePoints = new b2Vec2[m_allPointsSize];
	
	genCenterline(centerlinePoints);
	genLogicalTrackRepresentation(centerlinePoints);
	genPhysicsTrackRepresentation();

	delete[] centerlinePoints;
}

void	CTrack::loadSettingsFromTOML(const char *filename)
{
	std::ifstream ifs(filename);
	toml::Parser parser(ifs);
	toml::Value documentRoot   = parser.parse();
	toml::Value* trackSettings = documentRoot.find("tracksettings");
	unsigned int k = 0;
	
	m_settings.track_size	 = trackSettings->find("track_size")->as<int>();
	m_settings.down_step	 = trackSettings->find("down_step")->as<int>();
	m_settings.sector_step   = trackSettings->find("sector_step")->as<int>();
	m_settings.track_width   = trackSettings->find("track_width")->as<int>();
	
	const toml::Array& radius_curvature = trackSettings->find("radius_curvature")->as<toml::Array>();
	k = 0;
	for (const toml::Value& v : radius_curvature)
		m_settings.radius_curvature[k++] = v.as<int>();

	m_settings.radius_offset_curvature = trackSettings->find("radius_offset_curvature")->as<int>();
	const toml::Array& hard_curvature_probability = trackSettings->find("hard_curvature_probability")->as<toml::Array>();
	k = 0;
	for (const toml::Value& v : hard_curvature_probability)
		m_settings.hard_curvature_probability[k++] = v.as<int>();

	m_settings.hard_curvature_value = trackSettings->find("hard_curvature_value")->as<int>();
	m_settings.smooth_iterations = trackSettings->find("smooth_iterations")->as<int>();

	const toml::Array& physics_wall_size_inner = trackSettings->find("physics_wall_size_inner")->as<toml::Array>();
	k = 0;
	for (const toml::Value& v : physics_wall_size_inner)
		m_settings.physics_wall_size_inner[k++] = v.as<int>();

	const toml::Array& physics_wall_size_outer = trackSettings->find("physics_wall_size_outer")->as<toml::Array>();
	k = 0;
	for (const toml::Value& v : physics_wall_size_outer)
		m_settings.physics_wall_size_outer[k++] = v.as<int>();

}