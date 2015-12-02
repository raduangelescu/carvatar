/*
 * glesProgram.cpp
 *
 *  Created on: 28.9.2011
 *      Author: Tom
 */

#include "stdafx.h"
#include "glesProgram.h"

#undef LOG_TAG
#define  LOG_TAG    "Font"

//------------------------------------------------------------------------
glesProgram::glesProgram()
{
	mProgram = -1;
}

//------------------------------------------------------------------------
glesProgram::~glesProgram()
{
	// detaches shaders
	glDeleteProgram(mProgram);

	mProgram = -1;
}

//------------------------------------------------------------------------
GLuint glesProgram::construct(const  char* aVtxSrc, const  char* aFragSrc)
{
	GLuint vtxShader = loadShader(GL_VERTEX_SHADER, aVtxSrc);
	GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, aFragSrc);

	// Create the program object
	mProgram = glCreateProgram();
	if (mProgram == 0)
		return false;

	glAttachShader(mProgram, vtxShader);
	glAttachShader(mProgram, fragShader);

	GLint linked;
	// Link the program
	glLinkProgram(mProgram);

	// Check the link status
	glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];
			glGetProgramInfoLog(mProgram, infoLen, NULL, infoLog);
			delete infoLog;
		}
		glDeleteProgram(mProgram);
		return -1;
	}

	// delete shaders
//	glDeleteShader(vtxShader);
//	glDeleteShader(fragShader);

	return mProgram;
}

//------------------------------------------------------------------------
GLuint glesProgram::construct(const  char* aVtxFile, int aVtxFileIdx,
		const  char* aFragFile, int aFragFileIdx)
{
	/*AssetLoader loader;

	loader.construct(aVtxFile, aVtxFileIdx >= 0, true);
	char* vtxSrc = loader.loadString(aVtxFileIdx, true);
	loader.construct(aFragFile, aFragFileIdx >= 0, true);
	char* fragSrc = loader.loadString(aFragFileIdx, true);
	*
	GLuint programID = construct(vtxSrc, fragSrc);

	delete vtxSrc;
	delete fragSrc;

	return programID;
	*/
	return 0;
}

//------------------------------------------------------------------------
// type = GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
GLuint glesProgram::loadShader(GLenum aType, const  char* aShaderSrc)
{
	GLuint shader;	// returned handle

	// Create the shader object
	shader = glCreateShader(aType);
	if (shader == 0) return 0;

	// Load the shader source - shader source must be null terminated!
	glShaderSource(shader, 1, &aShaderSrc, NULL);

	GLint compiled;	// state of compilation
	// Compile the shader
	glCompileShader(shader);
	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			delete infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

//------------------------------------------------------------------------
GLuint glesProgram::getID()
{
	return mProgram;
}

//------------------------------------------------------------------------
GLuint glesProgram::getUniformLocation(const  char* aUniformName)
{
	return glGetUniformLocation(mProgram, aUniformName);
}

//------------------------------------------------------------------------
GLuint glesProgram::getAttribLocation(const  char* aAttributeName)
{
	return glGetAttribLocation(mProgram, aAttributeName);
}

//------------------------------------------------------------------------
void glesProgram::printActiveAttribs()
{
	GLint maxLength, nAttribs;

	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	 char * name = new  char[maxLength];

	GLint written, size, location;
	GLenum type;
	for(int i = 0; i < nAttribs; i++ )
	{
	    glGetActiveAttrib(mProgram, i, maxLength, &written, &size, &type, name );
	    location = glGetAttribLocation(mProgram, name);
	}

	delete name;
}

//------------------------------------------------------------------------
void glesProgram::printActiveUniforms()
{
	GLint nUniforms, maxLength;

	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &nUniforms);

	 char* name = new  char[maxLength];

	GLint size, location;
	GLsizei written;
	GLenum type;
	for(int i = 0; i < nUniforms; ++i )
	{
	    glGetActiveUniform(mProgram, i, maxLength, &written,&size, &type, name );
	    location = glGetUniformLocation(mProgram, name);
	}

	delete name;
}
