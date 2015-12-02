#include "stdafx.h"
#include "Track.h"

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

void CTrack::clear()
{
	//g_physicsInfo.world->DestroyBody(m_groundBody);
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

void CTrack::genTrack(unsigned int size, unsigned int downStep)
{
	m_finishLineRaceSectorIdx = 0;
	m_allPointsSize = size;
	m_downStep		= downStep;
	m_trackLength	= 0.0f;
	m_sectorStep	=	2;

	m_points = new SGenTrackNode[m_allPointsSize];
	//set up ground areas
	{
		b2BodyDef bodyDef;
		m_groundBody = g_physicsInfo.world->CreateBody(&bodyDef);
		
		//general shape
		b2Vec2 CenterOffset = b2Vec2(SCREEN_SIZE_WIDTH / 2.0f, SCREEN_SIZE_HEIGHT / 2.0f);

		for (unsigned int i = 0; i < m_allPointsSize; i += m_downStep)
		{
			m_points[i].width = 80;//randomInterval(60, 70);

			float RandomRadius = randomInterval(0, 50) - 20.0f;
			float offsetconst = 300;
			if (randomInterval(0, 3) >= 2.0)
			{
				offsetconst = 20;
			}
			float RadiusInner = offsetconst + RandomRadius;
			float RadiusOuter = RadiusInner + m_points[i].width;

			float alpha = ((360.0f / (m_allPointsSize - 1)) * i) * DEGTORAD;


			m_points[i].outer = CenterOffset + RadiusOuter * b2Vec2(sin(alpha), cos(alpha));
			m_points[i].inner = CenterOffset + RadiusInner * b2Vec2(sin(alpha), cos(alpha));
			m_points[i].center = CenterOffset + ((RadiusInner + RadiusOuter) / 2.0f) * b2Vec2(sin(alpha), cos(alpha));

		}

	
		
		for (unsigned int i = 1; i < m_allPointsSize; i += m_downStep)
		{
				b2Vec2 dir = (m_points[i].center - m_points[i - 1].center);
				m_trackLength += dir.Length();
				dir.Normalize();
				m_points[i].direction = dir;
		}
		//add stuff end
		m_points[m_allPointsSize - 1] = m_points[0];

		m_points[0].direction = (m_points[1].center - m_points[0].center);
		m_points[0].direction.Normalize();

		unsigned int k = 0;
		float len = (float)m_downStep;
		int lastidx = 0;
		for (unsigned int i = 0; i < m_allPointsSize; i++)
		{
			if (i != k)
			{
				float ammount = 1.0f - ((k - i) / (float)len);

				m_points[i].center = Lerp(m_points[lastidx].center, m_points[k].center, ammount);
				m_points[i].outer =  Lerp(m_points[lastidx].outer, m_points[k].outer, ammount);
				m_points[i].inner =  Lerp(m_points[lastidx].inner, m_points[k].inner, ammount);

				m_points[i].direction = (m_points[k].center - m_points[i].center);
				m_points[i].direction.Normalize();

			}
			else
			{
				k += m_downStep;
				if (k >= m_allPointsSize)
				{
					k = m_allPointsSize - 1;

				}
				len =(float)( k - i);
				lastidx = i;
			}
		}

	
		// smooth points
		const int smooth_iter = 400;
		for (int i = 0; i < smooth_iter; i++)
		{
			for (unsigned int k = 0; k < m_allPointsSize; k++)
			{
				int prev_idx = k - 1;
				int next_idx = k + 1;
				if (prev_idx < 0)
					prev_idx = m_allPointsSize-1;
				if (next_idx >= m_allPointsSize )
					next_idx = 0;

				m_points[k].outer = 1/3.f *(m_points[prev_idx].outer + m_points[k].outer + m_points[next_idx].outer);
				m_points[k].inner = 1/3.f*(m_points[prev_idx].inner + m_points[k].inner  + m_points[next_idx].inner);
				m_points[k].center = 1/3.f *(m_points[prev_idx].center + m_points[k].center + m_points[next_idx].center);
			}
		}

		// generate circuit phisic walls
		for (unsigned int i = 0; i < m_allPointsSize; i++)
		{
			
			b2PolygonShape wallShapeInner;
			wallShapeInner.SetAsBox(5.f, 5., m_points[i].inner, atan2(m_points[i].direction.y, m_points[i].direction.x));
			
			b2FixtureDef wallfixtureDefInner;
			wallfixtureDefInner.shape = &wallShapeInner;

			b2Fixture* wallFixtureInner = m_groundBody->CreateFixture(&wallfixtureDefInner);
			
			b2PolygonShape wallShapeOuter;
			wallShapeOuter.SetAsBox(5., 5., m_points[i].outer, atan2(m_points[i].direction.y, m_points[i].direction.x));

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

}