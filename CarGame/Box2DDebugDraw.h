/*
 * Box2DDebugDraw.h
 *
 *  Created on: 16.11.2012
 *      Author: Tom
 */
#pragma once

#include "Vector2.h"

#include <Box2D/Box2D.h>
#include "glesProgram.h"

struct b2AABB;

struct DebugLine
{
	b2Vec2 p1, p2;
};

class Box2DDebugDraw : public b2Draw
{
private:
	enum
	{
		eTriangles = 0x01,
		eLines     = 0x02,
		ePoints    = 0x04
	};

public:

	std::vector<DebugLine> debug_lines;

	static const unsigned int MAX_VERTICES = 64;
	static const unsigned int CIRCLE_SEGMENTS = 16;

public:
	Box2DDebugDraw(float aRatio);
	~Box2DDebugDraw();
	void construct();

public:
	void DrawPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor);
	void DrawSolidPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor);
	void DrawCircle(const b2Vec2& aCenter, float32 aRadius, const b2Color& aColor);
	void DrawSolidCircle(const b2Vec2& aCenter, float32 aRadius, const b2Vec2& aAxis, const b2Color& aColor);
	void DrawSegment(const b2Vec2& aP1, const b2Vec2& aP2, const b2Color& aColor);
	void DrawTransform(const b2Transform& aXf);
	void DrawPoint(const b2Vec2& aP, float32 aSize, const b2Color& aColor);
	void DrawString(int aX, int aY, const unsigned char* aString, ...);
	void DrawAABB(b2AABB* aAabb, const b2Color& aColor);

private:
	void createPolygonVertices(const b2Vec2* aVertices, int32 aVertexCount);
	void createCircleVertices(const b2Vec2& aCenter, float32 aRadius);
	void drawPrimitives(unsigned int aPrimitiveTypes, unsigned int aCount, const b2Color& aColor);

private:
	float mRatio;
	fvec2 mVertices[MAX_VERTICES];
	float mPointSize;

	glesProgram* mProgram;
	GLuint mColorLocation;
	GLuint mPointSizeLocation;
	GLuint mPositionLocation;
};

