//created by Ziyad Barakat 2016

#ifndef TINY_SAMPLERS_H
#define TINY_SAMPLERS_H

#if defined(_WIN32) || defined(_WIN64)
#include <gl/GL.h>

//disable warnings about unsafe stdio functions
#pragma warning(disable: 4474)
#pragma warning(disable: 4996)
//this automatically loads the OpenGL library if you are using Visual Studio
//comment this out if you have your own method
#pragma comment (lib, "opengl32.lib")
#endif

#if defined(__linux__)
#include <GL/gl.h>
#endif

#include <list>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define TINYSAMPLERS_ERROR_NOT_INITIALIZED 1
#define TINYSAMPLERS_ERROR_INVALID_STRING 2
#define TINYSAMPLERS_ERROR_INVALID_SAMPLER_NAME 3
#define TINYSAMPLERS_ERROR_INVALID_SAMPLER_INDEX 4
#define TINYSAMPLERS_ERROR_INVALID_FILE_PATH 5
#define TINYSAMPLERS_ERROR_SAMPLER_NOT_FOUND 6
#define TINYSAMPLERS_ERROR_FAILED_SAMPLER_LOAD 7
#define TINYSAMPLERS_ERROR_SAMPLER_ALREADY_EXISTS 8
#define TINYSHADERS_ERROR_INVALID_SOURCE_FILE 9

#define TINYSAMPLERS_DEFAULT_SAMPLER_PATH "./Samplers/"
#define TINYSAMPELRS_DEFAULT_SAMPLER_CONFIG_PATH "Samplers.txt"

inline static void TinySamplers_PrintErrorMessage(GLuint errorNumber, const char* errorMessage = nullptr)
{
	switch (errorNumber)
	{
	case TINYSAMPLERS_ERROR_NOT_INITIALIZED:
	{
		printf("Error: TinySamplers must first be initialized \n");
		break;
	}

	case TINYSAMPLERS_ERROR_INVALID_STRING:
	{
		printf("Error: given sampler name is invalid \n");
		break;
	}

	case TINYSAMPLERS_ERROR_INVALID_SAMPLER_INDEX:
	{
		printf("Error: given sampler index is invalid \n");
		break;
	}

	case TINYSAMPLERS_ERROR_INVALID_SAMPLER_NAME:
	{
		printf("Error: given sampler name is invalid: %s \n", errorMessage);
		break;
	}

	case TINYSAMPLERS_ERROR_INVALID_FILE_PATH:
	{
		printf("Error: given file path is invalid: %s \n", errorMessage);
		break;
	}

	case TINYSAMPLERS_ERROR_SAMPLER_NOT_FOUND:
	{
		printf("Error: sampler with given name %s was not found \n", errorMessage);
		break;
	}

	case TINYSAMPLERS_ERROR_SAMPLER_ALREADY_EXISTS:
	{
		printf("Error: sampler with given naame %s already exists \n", errorMessage);
		break;
	}

	case TINYSAMPLERS_ERROR_INVALID_SOURCE_FILE:
	{
		printf("Given source file %s is invalid \n");
		break;
	}

	default:
	{
		break;
	}

	}
}

class tinySamplers
{
	struct sampler_t;

public:

	tinySamplers( void ) {}
	~tinySamplers( void ) {}

	/*
	 * shuts down TinySamplers. deletes all OpengL texture samplers and clears the vector
	 */
	static inline void ShutDown(void)
	{

	}

private:

	struct sampler_t
	{
		const char* name;
		const char* filePath;


	};
};

#endif