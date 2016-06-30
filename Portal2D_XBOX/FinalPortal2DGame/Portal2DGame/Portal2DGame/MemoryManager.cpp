#define MEMORY_MANAGER_CPP
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
#include "MemoryManager.h"

MemoryManagerC* MemoryManagerC::sInstance = NULL;


MemoryManagerC *MemoryManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new MemoryManagerC();
	return sInstance;
}
void MemoryManagerC::init()
{
	sInstance->mBytesAllocated = 0;
	sInstance->mBytesFreed = 0;
}

template<class T> T *MemoryManagerC::MemAlloc(T type)
{
	T *retVal = new (std::nothrow) T;

	if (retVal == NULL)
	{
		printf("MAJOR_ERROR : Unable to allocate memory");
	}

	sInstance->mBytesAllocated += sizeof(T);
	return retVal;
}

template<class T> void MemoryManagerC::MemFree(T *ptr)
{
	if (ptr)
	{
		delete ptr;
	}
	else
	{
		printf("MAJOR_LOG: Trying to free up a NULL Ptr");
	}

	sInstance->mBytesFreed += sizeof(T);
}

void MemoryManagerC::PrintMemoryStatistics()
{
	printf("Bytes Allocated by the memory Manager = %d", sInstance->mBytesAllocated);
	printf("Bytes Freed up by the memory Manager = %d", sInstance->mBytesFreed);

	printf("Unfreed Memory = %d", sInstance->mBytesAllocated - sInstance->mBytesFreed);
}

void MemoryManagerC::shutdown()
{
}