#define FILE_MANAGER_CPP
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
#include "FileManager.h"

#include "soil.h"

FileManagerC* FileManagerC::sInstance = NULL;


FileManagerC *FileManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new FileManagerC();
	return sInstance;
}


void FileManagerC::ReadLevelDataFile()
{
	//Read the Level Data file and give the data to the Level Manager
}

void FileManagerC::ReadAudioFile()
{
	//Read the audio file and give it to the Audio Manager
}

void FileManagerC::OpenDebugFile()
{
	//Open the debug file and give to the debug manager?
}

void FileManagerC::shutdown()
{
	//Close all the files that are open
}