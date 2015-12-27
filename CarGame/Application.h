#pragma once

class CTrack;
class Box2DDebugDraw;
class RaceManager;

class Application
{
	RenderData			m_renderInfo;
	PhysicsData			m_physicsInfo;
	CTrack*				m_trackInfo;
	RaceManager*		m_raceManager;

	unsigned int		m_randomRoot;
	bool				m_isRunning;

private:
	void initPhyisics();
	void initRender();
	void drawDebugLines();
	void updateEvents();
	void updateAi();

public:
	Application();
	void init();
	void step();
	void draw();
	void update();
	void run();
	~Application();

	RenderData * getRender() { return &m_renderInfo; }
	PhysicsData* getPhysics() { return &m_physicsInfo; }
	CTrack*		 getTrack() { return m_trackInfo; }
	RaceManager* getRaceManager() { return m_raceManager; }
};

#define RENDER g_Application->getRender()
#define PHYSX  g_Application->getPhysics()
#define TRACK  g_Application->getTrack()
#define RACE   g_Application->getRaceManager()

extern Application *g_Application;

