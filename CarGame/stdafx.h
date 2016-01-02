// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef _PRECOMP_H
#define _PRECOMP_H

#define _CRT_SECURE_NO_DEPRECATE
// Activate memory leak detection in debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "targetver.h"

#define timegm _mkgmtime
#include <time.h> 
#include <tinytoml.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <GL/glew.h>
#include <SDL.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <set>
#include <apiset.h>
#include <glm/glm.hpp>
#include <fstream>



#define LOG_MAX_SIZE 1024

#define SCREEN_SIZE_WIDTH (1980)
#define SCREEN_SIZE_HEIGHT (1080)

enum INPUT_SENSOR_TYPE
{
	IS_VELOCITY,
	IS_LEFTDISTANCE,
	IS_TRACKANGLE,
	IS_RAYCAST90,
	IS_RAYCAST45,
	IS_RAYCAST0,
	IS_RAYCASTM45,
	IS_RAYCASTM90,
	IS_CARDISTANCETOCENTERLINE,

	IS_NUM
};

enum FITNESS_TYPE
{
	FT_NUMCRASHES,
	FT_INVERSELAPTIME,
	FT_NUM
};
//key pressed
enum OUTPUT_ACTION_TYPE
{
	OA_UP,
	OA_DOWN,
	OA_LEFT,
	OA_RIGHT,

	OA_NUM
};

class Box2DDebugDraw;

struct RenderData
{
	SDL_Window*		window;
	SDL_Renderer*	render;
	Box2DDebugDraw*	ddraw;
};

struct PhysicsData
{
	b2World * world;
};

enum COLLISION_FILTERS
{
	CATEGORY_CAR = 0x0001,
	CATEGORY_STATIC = 0x0002
};

//types of fixture user data
enum fixtureUserDataType
{
	FUD_CAR,
	FUD_RACE_SECTOR
};

//a class to allow subclassing of different fixture user data
class FixtureUserData
{
	fixtureUserDataType m_type;
protected:
	FixtureUserData(fixtureUserDataType type);
public:
	virtual fixtureUserDataType getType();
	virtual ~FixtureUserData();
};


void prinfvector(float *v, unsigned int size);
void fprinfvector(FILE*f, float *v, unsigned int size);
float randomInterval(int min, int max);
b2Vec2 lerp(b2Vec2 start, b2Vec2 end, float alpha);

void checkSDLError(int line = -1);
void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, 
										GLenum severity, GLsizei length, const GLchar* message,
										const void* userParam);

void RotateVector(b2Vec2 vec, float angle, b2Vec2 &output);
inline void Log(const char* fmt, ...)
{
	char buf[LOG_MAX_SIZE];
	va_list ap;
	va_start(ap, fmt);
	_vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);
	buf[LOG_MAX_SIZE - 1] = 0;
	printf(buf);
}

#ifdef USE_LOGGER
#define LOG(X, ...) Log(X,  __VA_ARGS__)
#else
#define LOG(fmt, ...) 
#endif
#endif
