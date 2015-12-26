#include "stdafx.h"
#include "Box2DDebugDraw.h"


//------------------------------------------------------------------------
Box2DDebugDraw::Box2DDebugDraw(float aR) : mRatio(aR)
{
	mPointSize = 1.0f;
	mProgram = NULL;
	mColorLocation = 0;
	mPointSizeLocation = 0;
	mPositionLocation = 0;
}

//------------------------------------------------------------------------
Box2DDebugDraw::~Box2DDebugDraw()
{
	if (mProgram != NULL)
	{
		delete mProgram;
		mProgram = NULL;
	}
}

//------------------------------------------------------------------------
void Box2DDebugDraw::construct()
{
	static const  char *vertexShader = "\
        uniform mat4 u_projection;\
		uniform float u_pointSize;\
	    attribute vec2 a_position;\
	    void main() {\
		  gl_PointSize = u_pointSize;\
		  vec4 position = vec4(a_position, 0.0, 1.0);\
	      gl_Position = u_projection * position;\
	    }\
	    ";

	static const  char *fragmentShader = "\
	    precision mediump float; \
        uniform vec4 u_color;\
		void main() { \
	      gl_FragColor = u_color * vec4(1.0,1.0,0.0,1.0); \
	    } \
	    ";

	GLfloat projection[16] = {30.0f / SCREEN_SIZE_WIDTH, 0                        , 0, 0,
							  0                       , 30.0f / SCREEN_SIZE_HEIGHT, 0, 0,
							  0                       , 0                        , 0, 0,
							  -15                      , -15                       , 0, 1};
	//glm::mat4X4 projection = glm::ortho
	// create program
	mProgram = new glesProgram();
	mProgram->construct(vertexShader, fragmentShader);
	// cache color uniform location
	mColorLocation = mProgram->getUniformLocation("u_color");
	mPointSizeLocation = mProgram->getUniformLocation("u_pointSize");
	mPositionLocation = mProgram->getAttribLocation("a_position");
	int projid = mProgram->getUniformLocation("u_projection");
	// use program and set projection matrix
	glUseProgram(mProgram->getID());
	glUniformMatrix4fv(projid,
			1, GL_FALSE, (GLfloat*) &projection[0]);
	glUseProgram(0);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor)
{
	createPolygonVertices(aVertices, aVertexCount);
	drawPrimitives(eLines, aVertexCount, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor)
{
	createPolygonVertices(aVertices, aVertexCount);
	drawPrimitives(eTriangles + eLines, aVertexCount, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawCircle(const b2Vec2& aCenter, float32 aRadius, const b2Color& aColor)
{
	createCircleVertices(aCenter, aRadius);
	drawPrimitives(eLines, CIRCLE_SEGMENTS, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& aCenter, float32 aRadius, const b2Vec2& aAxis,
		const b2Color& aColor)
{
	createCircleVertices(aCenter, aRadius);
	drawPrimitives(eTriangles + eLines, CIRCLE_SEGMENTS, aColor);
	// Draw the axis line
	DrawSegment(aCenter, aCenter + aRadius * aAxis, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawSegment(const b2Vec2& aP1, const b2Vec2& aP2, const b2Color& aColor)
{
	mVertices[0].mX = aP1.x * mRatio;
	mVertices[0].mY = aP1.y * mRatio;
	mVertices[1].mX = aP2.x * mRatio;
	mVertices[1].mY = aP2.y * mRatio;
	drawPrimitives(eLines, 2, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawTransform(const b2Transform& aXf)
{
	b2Vec2 p1 = aXf.p, p2;
	const float32 k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * aXf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));

	p2 = p1 + k_axisScale * aXf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawPoint(const b2Vec2& aP, float32 aSize, const b2Color& aColor)
{
	mPointSize = aSize;

	mVertices[0].mX = aP.x * mRatio;
	mVertices[0].mY = aP.y * mRatio;
	drawPrimitives(ePoints, 1, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawString(int aX, int aY, const unsigned char * aString, ...)
{
	//LOGD("Not yet implemented");
}

//------------------------------------------------------------------------
void Box2DDebugDraw::DrawAABB(b2AABB* aAabb, const b2Color& aColor)
{
	mVertices[0].mX = aAabb->lowerBound.x * mRatio;
	mVertices[0].mY = aAabb->lowerBound.y * mRatio;
	mVertices[1].mX = aAabb->upperBound.x * mRatio;
	mVertices[1].mY = aAabb->lowerBound.y * mRatio;
	mVertices[2].mX = aAabb->upperBound.x * mRatio;
	mVertices[2].mY = aAabb->upperBound.y * mRatio;
	mVertices[3].mX = aAabb->lowerBound.x * mRatio;
	mVertices[3].mY = aAabb->upperBound.y * mRatio;
	drawPrimitives(eLines, 8, aColor);
}

//------------------------------------------------------------------------
void Box2DDebugDraw::createPolygonVertices(const b2Vec2* aVertices, int32 aVertexCount)
{
	if (aVertexCount > MAX_VERTICES)
	{
		//LOGE("more vertices then allocated space");
		return;
	}

	// convert vertices to screen resolution
	for (int i = 0; i < aVertexCount; i++)
	{
		mVertices[i].mX = mRatio * aVertices[i].x;
		mVertices[i].mY = mRatio * aVertices[i].y;
	}
}

//------------------------------------------------------------------------
void Box2DDebugDraw::createCircleVertices(const b2Vec2& aCenter, float32 aRadius)
{
	int vertexCount = 16;
	const float32 k_increment = 2.0f * b2_pi / CIRCLE_SEGMENTS;
	float32 theta = 0.0f;

	for (int32 i = 0; i < CIRCLE_SEGMENTS; ++i)
	{
		b2Vec2 v = aCenter + aRadius * b2Vec2(std::cos(theta), std::sin(theta));
		mVertices[i].mX = mRatio * v.x;
		mVertices[i].mY = mRatio * v.y;
		theta += k_increment;
	}
}

//------------------------------------------------------------------------
void Box2DDebugDraw::drawPrimitives(unsigned int aPrimitiveTypes, unsigned int aCount, const b2Color& aColor)
{

	glUseProgram(mProgram->getID());
	glEnableVertexAttribArray(mPositionLocation);
	glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*) mVertices);

	if (aPrimitiveTypes & eTriangles)
	{
		glUniform4f(mColorLocation, aColor.r, aColor.g, aColor.b, 0.5f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, aCount);
	}

	if (aPrimitiveTypes & eLines)
	{
		glUniform4f(mColorLocation, aColor.r, aColor.g, aColor.b, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, aCount);
	}

	if (aPrimitiveTypes & ePoints)
	{
		glUniform4f(mColorLocation, aColor.r, aColor.g, aColor.b, 1.0f);
		glUniform1f(mPointSizeLocation, mPointSize);
		glDrawArrays(GL_POINTS, 0, aCount);
	}

	glDisableVertexAttribArray(mPositionLocation);
	glUseProgram(0);

}

