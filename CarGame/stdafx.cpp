// stdafx.cpp : source file that includes just the standard includes
// CarGame.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void checkSDLError(int line)
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

void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	printf("%s\n", message);
}

void prinfvector(float *v, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		printf("%f ", v[i]);
	printf("\n");
}

void fprinfvector(FILE*f, float *v, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		fprintf(f, "%f ", v[i]);
	fprintf(f, "\n");
}

void RotateVector(b2Vec2 vec, float angle, b2Vec2 &output)
{
	float cs = cos(angle);
	float sn = sin(angle);

	output.x = vec.x * cs - vec.y * sn;
	output.y = vec.x * sn + vec.y * cs;

}
FixtureUserData::FixtureUserData(fixtureUserDataType type) : m_type(type)
{
}

fixtureUserDataType FixtureUserData::getType()
{
	return m_type;
}

FixtureUserData::~FixtureUserData()
{
}

float randomInterval(int min, int max)
{
	if (min == max)
		return (float)min;
	int offset = 0;
	if (min < 0)
	{
		max = max - min;
		offset = min;
		min = 0;

	}

	return (float)(rand() % max + min) + offset;
}

b2Vec2 lerp(b2Vec2 start, b2Vec2 end, float alpha)
{
	b2Vec2 dir = end - start;
	float length = dir.Normalize();

	return start + alpha * length * dir;
}
