#pragma once
#include "stdafx.h"
#include "Application.h"
#include "CarModel.h"
#include "Box2DDebugDraw.h"
#include "Track.h"
#include "Car.h"
#include "RaceManager.h"
#include "DifferentialEvolution.h"

class DestructionListener : public b2DestructionListener
{
	void SayGoodbye(b2Fixture* fixture)
	{
		FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData();

		if (fud != NULL)
			delete fud;
	}

	void SayGoodbye(b2Joint* joint)
	{

	}
}destructionListener;


Application::Application()
{
	m_physicsInfo.world = new b2World(b2Vec2(0.0f, 0.0f));
	m_renderInfo.ddraw = new Box2DDebugDraw(1.0f);
	m_trackInfo			= new CTrack();
	
	//m_raceManager = new RaceManager();
	m_raceManager = new DifferentialEvolution();

	m_isRunning = true;
}

void Application::initPhyisics()
{
	// init physics world 
	m_physicsInfo.world->SetDestructionListener(&destructionListener);

	m_renderInfo.ddraw->construct();
	m_physicsInfo.world->SetDebugDraw(m_renderInfo.ddraw);
	// set debug draw flags from available : 
	// b2Draw::e_shapeBit, b2Draw::e_jointBit. b2Draw::e_aabbBit, b2Draw::e_pairBit, b2Draw::e_centerOfMassBit
	m_renderInfo.ddraw->SetFlags(b2Draw::e_shapeBit);
	
}

void Application::initRender()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG("SDL_Init Error: %s \n", SDL_GetError());
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_CreateWindowAndRenderer(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT, SDL_WINDOW_OPENGL, &m_renderInfo.window, &m_renderInfo.render);

	if (m_renderInfo.render == nullptr)
	{
		LOG("SDL_CreateRenderer Error: %s \n", SDL_GetError());
		return;
	}
	SDL_GLContext context = SDL_GL_CreateContext(m_renderInfo.window);
	glewInit();
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGlDebugCallback, NULL);
	glEnable(GL_DEBUG_OUTPUT);
	
	glClearColor(1.0f, 1.0f, 1.0f, 100.0f);
	/* Clear the color and depth buffers. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);
}

void Application::init()
{
	initRender();
	initPhyisics();

	m_randomRoot = (unsigned int)time(NULL);
	srand(m_randomRoot);
	
	m_trackInfo->genTrack();
	m_raceManager->init();
	m_trackInfo->genPhysicsTrackRepresentation();

	m_physicsInfo.world->SetContactListener(m_raceManager);
}

void Application::step()
{
	m_raceManager->updateModels();
	m_physicsInfo.world->Step(1.0f / 30.0f, 8, 3);
}

void Application::drawDebugLines()
{
	b2Color red(1.0f, 0.0, 0.0);
	
	for (unsigned int i = 0; i < m_renderInfo.ddraw->debug_lines.size(); i++)
	{
		m_renderInfo.ddraw->DrawSegment(m_renderInfo.ddraw->debug_lines[i].p1, m_renderInfo.ddraw->debug_lines[i].p2, red);
	}
	m_renderInfo.ddraw->debug_lines.clear();
}

void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);

	//m_physicsInfo.world->DrawDebugData();
	m_trackInfo->debugDraw();
	m_raceManager->debugDraw();

	drawDebugLines();

	SDL_GL_SwapWindow(m_renderInfo.window);
}

void Application::updateEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		const unsigned char * state = SDL_GetKeyboardState(NULL);
	
		switch (event.type)
		{
			case	SDL_QUIT:
			{
				m_isRunning = false;
				break;
			}

			case SDL_KEYDOWN:
			{
				m_raceManager->keyEvent(event.key.keysym.sym, true);
				break;
			}

			case SDL_KEYUP:
			{
				m_raceManager->keyEvent(event.key.keysym.sym, false);
				break;
			}
		}
	}
}

void Application::updateAi()
{
	m_raceManager->updateControllers();
}

void Application::update()
{
	unsigned int start = SDL_GetTicks();

	updateEvents();
	updateAi();
	step();
	draw();

	// maintain constant update, should do better in case the update lags (accumulator)
	if (1000.0 / 30 > SDL_GetTicks() - start)
		SDL_Delay((unsigned int)(1000.0f / 30.0f) - (SDL_GetTicks() - start));

}

void Application::run()
{
	while (m_isRunning)
	{
		update();
	}
}

Application::~Application()
{
	m_trackInfo->clear();
	m_trackInfo->destroyPhysicsBody();
	
	delete m_trackInfo;
	delete m_raceManager;
	delete m_physicsInfo.world;
	delete m_renderInfo.ddraw;
	
	SDL_DestroyRenderer(m_renderInfo.render);
	SDL_DestroyWindow(m_renderInfo.window);
	SDL_Quit();
}

