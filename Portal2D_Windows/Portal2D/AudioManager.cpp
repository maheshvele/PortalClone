#define AUDIO_MANAGER_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "gl\glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "AudioManager.h"

AudioManagerC* AudioManagerC::sInstance = NULL;


AudioManagerC *AudioManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new AudioManagerC();
	return sInstance;
}
void AudioManagerC::init()
{
}

void AudioManagerC::update()
{

}

void AudioManagerC::shutdown()
{
}