// stdafx.cpp : source file that includes just the standard includes
// CarGame.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
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