#pragma once

class TopdownCar;

class Evaluator
{
public:
	Evaluator(TopdownCar *car);

	void start();

	void update(float fDelta);

	float getTime() const { return m_fTime; }
	float getPoints() const { return m_fPoints; }

	bool isRunning() const { return m_bRunning; }

private:
	float evaluateCarTrackDistance() const;

private:
	float m_fTime;
	float m_fPoints;
	bool m_bRunning;
	TopdownCar *m_car;
};