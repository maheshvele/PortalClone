#define RENDERING_MANAGER_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "gl\glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include  "game.h"
#include "gamedefs.h"
#include "SOIL.h"
#include "Box2D.h"
#include <list>
#include "ShapeDraw.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "AnimationManager.h"
#include "LevelManager.h"

RenderingManagerC* RenderingManagerC::sInstance = NULL;

RenderingManagerC *RenderingManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new RenderingManagerC();
	return sInstance;
}
void RenderingManagerC::init()
{
	////Access the file Manager and get the background image
	//mBackgroundTexture = SOIL_load_OGL_texture("level1.png",
	//											SOIL_LOAD_AUTO,
	//											SOIL_CREATE_NEW_ID,
	//											(SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT));

	//mPlayerTexture = SOIL_load_OGL_texture("bro.png",
	//										 SOIL_LOAD_AUTO,
	//										 SOIL_CREATE_NEW_ID,
	//										 (SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT));

}

void RenderingManagerC::update()
{
	int currentState = LevelManagerC::GetInstance()->GetCurrentState();

	if (currentState == LEVELSTATE_INIT_LOADLEVEL)
	{
		//Access the file Manager and get the background image
		mBackgroundTexture = SOIL_load_OGL_texture(LevelManagerC::GetInstance()->currentLevelData.levelImage,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			(SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT));

		mPlayerTexture = SOIL_load_OGL_texture("bro.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			(SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT));
	}

	DrawTexturedRect(sInstance->mBackgroundTexture, 0.0, 0.0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0.0, 1.0, 0.0, 1.0);

	DrawStaticLevelObjects();

	DrawRayCast();

	DrawPortal(true, sInstance->mPlayerTexture);

	DrawPortal(false, sInstance->mPlayerTexture);

	DrawCursor();

	DrawPlayer(sInstance->mPlayerTexture);

	DrawEnemy();

}

void RenderingManagerC::shutdown()
{

}
