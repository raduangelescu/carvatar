/*
 * glesProgram.h
 *
 *  Created on: 28.9.2011
 *      Author: Tom
 */

#pragma once
#include "Vector2.h"

class glesProgram
{
public:
	glesProgram();
	~glesProgram();

public:
	GLuint construct(const  char* aVtxSrc, const  char* aFragSrc);
	GLuint construct(const  char* aVtxFile, int aVtxFileIdx, const  char* aFragFile, int aFragFileIdx);
	GLuint getID();
	void use();
	GLuint getUniformLocation(const  char* aUniformName);
	GLuint getAttribLocation(const  char* aAttributeName);
	void printActiveAttribs();
	void printActiveUniforms();

private:
	GLuint loadShader(GLenum aType, const  char* aShaderSrc);

private:
	GLuint mProgram;
};
