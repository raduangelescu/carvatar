#include "stdafx.h"
#include "Track.h"
#include <tinytoml.h>
#include "Box2DDebugDraw.h"

extern PhysicsData g_physicsInfo;
extern Box2DDebugDraw* g_debugDraw;

RaceSectorFUD::RaceSectorFUD(int _idx) : FixtureUserData(FUD_RACE_SECTOR), idx(_idx){}

CTrack::CTrack()
{

}

void CTrack::destroyPhysicsBody()
{
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

float CTrack::getSectorDistanceToCenterline(const unsigned int idx, b2Vec2 carPos)
{
	unsigned int numSectors = m_allPointsSize / m_sectorStep;
	unsigned currentidx = (idx % numSectors)* m_sectorStep;
	unsigned nextIdx    = ((idx + 1) % numSectors) * m_sectorStep;
	// go into track sector position system;
	b2Vec2 carPosInTrackSector = carPos - m_points[nextIdx].center;
	//carPosInTrackSector.Normalize();
	b2Vec2 diff = m_points[nextIdx].center - m_points[currentidx].center;
	b2Vec2 ortho;

	ortho.Set(-diff.y, diff.x);
	ortho.Normalize();

	float dot = ortho.x * carPosInTrackSector.x + ortho.y * carPosInTrackSector.y;
	float proj = dot/ m_settings.track_width;
	

	DebugLine l;
	l.p1 = m_points[nextIdx].center;
	l.p2 = l.p1 + proj * m_settings.track_width * ortho ;

	g_debugDraw->debug_lines.push_back(l);


	printf("%f value\n", proj);

	return proj;
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

void CTrack::lerpInterval(int startIdx, int endIdx, b2Vec2* points, int size)
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
	//Make the end point be the start point
	points[m_settings.track_size - 1] = points[0];
	//lerp last part if track_size is not a multiple of downstep
	if (i >= m_settings.track_size)
	{
		lerpInterval(i - m_settings.down_step, m_settings.track_size - 1, points, m_settings.track_size);
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

void CTrack::genPhysicsTrackRepresentation()
{
	b2BodyDef bodyDef;
	m_groundBody = g_physicsInfo.world->CreateBody(&bodyDef);
	b2Filter filter;
	filter.categoryBits = CATEGORY_STATIC;
	filter.maskBits = (uint16)(-1);

	// generate circuit phisic walls
	for (unsigned int i = 0; i < m_allPointsSize-2; i++)
	{
		unsigned int nextIdx = (i + 1);
		if (nextIdx >= m_allPointsSize - 2)
			nextIdx = 0;

		b2EdgeShape wallShapeInner;
		wallShapeInner.Set(m_points[i].inner, m_points[nextIdx].inner);
	
		b2FixtureDef wallfixtureDefInner;
		wallfixtureDefInner.shape = &wallShapeInner;

		b2Fixture* wallFixtureInner = m_groundBody->CreateFixture(&wallfixtureDefInner);
		wallFixtureInner->SetFilterData(filter);
	
		
		b2EdgeShape wallShapeOuter;
		wallShapeOuter.Set(m_points[i].outer, m_points[nextIdx].outer);

		
		b2FixtureDef wallfixtureDefOuter;
		wallfixtureDefOuter.shape = &wallShapeOuter;
		b2Fixture* wallFixtureOuter = m_groundBody->CreateFixture(&wallfixtureDefOuter);
		wallFixtureOuter->SetFilterData(filter);
	}

	unsigned int numSectors = m_allPointsSize / m_sectorStep;

	for (unsigned int sector = 0; sector < numSectors; sector++)
	{
		unsigned int currentIdx = sector * m_sectorStep;
		unsigned int nextIdx    = ((sector+1) % numSectors) * m_sectorStep;

		b2PolygonShape groundSectorShape;
		//Generate ground sector shape
		b2FixtureDef groundSectorFixtureDef;
		b2Vec2 quad[4];
		quad[0] = m_points[currentIdx].inner;
		quad[1] = m_points[currentIdx].outer;
		quad[2] = m_points[nextIdx].outer;
		quad[3] = m_points[nextIdx].inner;
		groundSectorShape.Set(quad,4);
		groundSectorFixtureDef.isSensor = true;
		groundSectorFixtureDef.shape = &groundSectorShape;

		b2Fixture* groundSectorFixture = m_groundBody->CreateFixture(&groundSectorFixtureDef);
		RaceSectorFUD *userData = new RaceSectorFUD(m_finishLineRaceSectorIdx);
		groundSectorFixture->SetUserData(userData);
		groundSectorFixture->SetFilterData(filter);

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

void CTrack::loadSettingsFromTOML(const char *filename)
{
	std::ifstream ifs(filename);
	toml::Parser parser(ifs);
	toml::Value documentRoot   = parser.parse();
	toml::Value* trackSettings = documentRoot.find("tracksettings");
	
	m_settings.track_size				= trackSettings->find("track_size")->as<int>();
	m_settings.down_step				= trackSettings->find("down_step")->as<int>();
	m_settings.sector_step				= trackSettings->find("sector_step")->as<int>();
	m_settings.track_width				= trackSettings->find("track_width")->as<int>();
	m_settings.radius_offset_curvature  = trackSettings->find("radius_offset_curvature")->as<int>();
	m_settings.hard_curvature_value     = trackSettings->find("hard_curvature_value")->as<int>();
	m_settings.smooth_iterations        = trackSettings->find("smooth_iterations")->as<int>();

	const toml::Array& radius_curvature				= trackSettings->find("radius_curvature")->as<toml::Array>();
	const toml::Array& hard_curvature_probability   = trackSettings->find("hard_curvature_probability")->as<toml::Array>();
	const toml::Array& physics_wall_size_inner		= trackSettings->find("physics_wall_size_inner")->as<toml::Array>();
	const toml::Array& physics_wall_size_outer		= trackSettings->find("physics_wall_size_outer")->as<toml::Array>();

	m_settings.radius_curvature[0] = radius_curvature.at(0).as<int>();
	m_settings.radius_curvature[1] = radius_curvature.at(1).as<int>();

	m_settings.hard_curvature_probability[0] = hard_curvature_probability.at(0).as<int>();
	m_settings.hard_curvature_probability[1] = hard_curvature_probability.at(1).as<int>();
	m_settings.hard_curvature_probability[2] = hard_curvature_probability.at(2).as<int>();
	
	m_settings.physics_wall_size_inner[0] = physics_wall_size_inner.at(0).as<double>();
	m_settings.physics_wall_size_inner[1] = physics_wall_size_inner.at(1).as<double>();		

	m_settings.physics_wall_size_outer[0] = physics_wall_size_outer.at(0).as<double>();
	m_settings.physics_wall_size_outer[1] = physics_wall_size_outer.at(1).as<double>();
	
}