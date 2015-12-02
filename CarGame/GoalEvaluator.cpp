#include "stdafx.h"
#include "GoalEvaluator.h"
#include "Track.h"
#include "Car.h"

extern CTrack*			g_trackInfo;

const float CHECK_DELTA = 0.5f;
const float PENALTY_MULTIPLIER = 10.0f;
const float OUTROAD_PENALTY = 1000.0f * PENALTY_MULTIPLIER;

Evaluator::Evaluator(TopdownCar *car):
	m_fTime(0.0f)
	, m_fPoints(0.0f)
	, m_bRunning(false)
	, m_car(car)
{
}

void Evaluator::start()
{
	m_fTime = 0.0f;
	m_fPoints = 0.0f;
	m_bRunning = true;
}

void Evaluator::update(float fDelta)
{
	if (m_bRunning)
	{
		m_fTime += fDelta;

		m_fPoints += evaluateCarTrackDistance() * fDelta;
		unsigned int carracesector = m_car->m_currentRaceSectorIdx;
		float distanceToFinish = g_trackInfo->getDistanceToFinishLine(carracesector);
		float totalRaceDistance = g_trackInfo->getTotalTrackLength();
		SGenTrackNode &currentTrackPoint = g_trackInfo->getTrackPoint(carracesector);
		float travelledDistance = totalRaceDistance - distanceToFinish;
	
		m_fPoints += (g_trackInfo->getTotalTrackLength() - travelledDistance) * m_fTime * PENALTY_MULTIPLIER;
		m_fPoints += (currentTrackPoint.center - m_car->getPosition()).Length() * m_fTime * PENALTY_MULTIPLIER;

		if (travelledDistance >= totalRaceDistance)
		{
			m_bRunning = false;
		}
	}
}

float Evaluator::evaluateCarTrackDistance() const
{
	/*float fCarTrackDistance = m_track.getCurrentDistanceFromTrack();

	return fCarTrackDistance > m_track.getTrackWidth() ? fCarTrackDistance * OUTROAD_PENALTY : fCarTrackDistance;*/
	return 0.0f;
}