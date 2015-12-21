// CarGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "defines.h"
#include "CarModel.h"
#include "Box2DDebugDraw.h"
#include "Track.h"
#include "Car.h"
#include "PlayerController.h"
#include "AINNController.h"
#include "BasicAIController.h"

#include <Windows.h>

RenderData		g_renderInfo;
PhysicsData		g_physicsInfo;
CTrack*			g_trackInfo;
Box2DDebugDraw* g_debugDraw;
IController *   g_controller;

class TopdownCarDestructionListener : public b2DestructionListener
{
	void SayGoodbye(b2Fixture* fixture)
	{
		FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData();
		
		if(fud != NULL)
			delete fud;
	}
	void SayGoodbye(b2Joint* joint)
	{

	}

}g_destructionListener;

void Track_init()
{
	/* initialize random seed: */
	//1213012
	srand((unsigned int)time(NULL));
	//1213013
	g_trackInfo = new CTrack();
	g_trackInfo->genTrack();
}

void Track_end()
{
	g_trackInfo->clear();
	delete g_trackInfo;
}

void Physics_init()
{
	g_physicsInfo.world = new b2World(b2Vec2(0.0f, -9.8f));
	g_debugDraw = new Box2DDebugDraw(1.0f);
	g_debugDraw->construct();
	g_physicsInfo.world->SetDebugDraw(g_debugDraw);
	g_physicsInfo.world->SetDestructionListener(&g_destructionListener);
	g_debugDraw->SetFlags(b2Draw::e_shapeBit /* + b2Draw::e_jointBit + b2Draw::e_aabbBit +
													5:                 b2Draw::e_pairBit + b2Draw::e_centerOfMassBit*/);

}

void Physics_step()
{
	g_physicsInfo.world->Step(1.0f / 30.0f , 8 , 3);
	
	g_controller->fixedStepUpdate();
	
}

void Physics_end()
{
	delete g_controller;
	delete g_physicsInfo.world;
}

void checkSDLError(int line = -1)
{
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}

}


static void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	printf("%s\n", message);

}

void Render_init()
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


	

	SDL_CreateWindowAndRenderer(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT, SDL_WINDOW_OPENGL, &g_renderInfo.window, &g_renderInfo.render);
	
	if (g_renderInfo.render == nullptr)
	{
		LOG("SDL_CreateRenderer Error: %s \n", SDL_GetError());
		return;
	}


	SDL_GLContext context = SDL_GL_CreateContext(g_renderInfo.window);

	
	glewInit();

	if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
		exit(1); // or handle the error in a nicer way

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGlDebugCallback, NULL);
	glEnable(GL_DEBUG_OUTPUT);


	glClearColor(0.0f, 0.0f, 0.0f, 100.0f);
	/* Clear the color and depth buffers. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);
	
}


void Render_drawFrame()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);

	g_physicsInfo.world->DrawDebugData();
	
	b2Color red(1.0f, 0.0, 0.0);
	for (unsigned int i = 0; i < g_debugDraw->debug_lines.size(); i++)
	{
		g_debugDraw->DrawSegment(g_debugDraw->debug_lines[i].p1, g_debugDraw->debug_lines[i].p2, red);
	}
	g_debugDraw->debug_lines.clear();
	SDL_GL_SwapWindow(g_renderInfo.window);

}


void Render_end()
{
	delete g_debugDraw;
	SDL_DestroyRenderer(g_renderInfo.render);
	SDL_DestroyWindow(g_renderInfo.window);
	SDL_Quit();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Memory leak detection
#ifdef _DEBUG
	//_crtBreakAlloc = 490;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Render_init();


	Physics_init();

	TopdownCar * car = new TopdownCar();

	Track_init();

	car->setPosition(g_trackInfo->getTrackPoint(0).center);

	//g_controller = new PlayerController();
	g_controller = new AINNController();
	//g_controller = new BasicAIController();

	g_controller->initController(car);


	SDL_Event event;

	bool IsRunning = true;
	g_physicsInfo.world->SetContactListener(car);
	while (IsRunning)
	{
		unsigned int start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			const unsigned char * state = SDL_GetKeyboardState(NULL);


			switch (event.type)
			{
				case	SDL_QUIT:
				{
					IsRunning = false;
					break;
				}
				
				case SDL_KEYDOWN:
				{
					g_controller->keyDown(event.key.keysym.sym);
					break;
				}
				case SDL_KEYUP:
				{
					g_controller->keyUp(event.key.keysym.sym);
					break;
				}
			}
		}
		Render_drawFrame();
		car->step();
		Physics_step();
		if (1000.0 / 30 > SDL_GetTicks() - start)
			SDL_Delay((unsigned int)(1000.0f / 30.0f) - (SDL_GetTicks() - start));
	}


	Render_end();
	g_trackInfo->destroyPhysicsBody();
	delete car;

	Track_end();
	Physics_end();
	
	LOG("Program normal termination.");
	return 0;
}

