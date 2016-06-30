#define GAME_CPP
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
#include "collInfo.h"
#include "object.h"
#include "ball.h"
#include "field.h"
#include "random.h"
#include "gameObjects.h"
#include "openGLStuff.h"
#include "game.h"
#include "StateManager.h"
#include "BallManager.h"
#include "FieldManager.h"
#include "InputManager.h"
#include <list>
#include "Box2D.h"
#include "LevelManager.h"
#include "PhysicsManager.h"
#include "SpriteDemoManager.h"
#include "RenderingManager.h"
#include "AnimationManager.h"

// Declarations
const char8_t CGame::mGameTitle[]="Framework1";
CGame* CGame::sInstance=NULL;
BOOL Initialize (GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
	initOpenGLDrawing(window,keys,0.0f, 0.0f, 0.0f);
	CGame::CreateInstance();
	CGame::GetInstance()->init();
	return TRUE;						
}

void CGame::init()
{
	InputManagerC::CreateInstance();
	PhysicsManagerC::CreateInstance();
	RenderingManagerC::CreateInstance();
	AnimationManagerC::CreateInstance();
	LevelManagerC::CreateInstance();

	LevelManagerC::GetInstance()->init();
	InputManagerC::GetInstance()->init();
	PhysicsManagerC::GetInstance()->init();
	RenderingManagerC::GetInstance()->init();
	AnimationManagerC::GetInstance()->init();
}
void CGame::UpdateFrame(DWORD milliseconds)			
{
	LevelManagerC::GetInstance()->update();
	InputManagerC::GetInstance()->update();
	PhysicsManagerC::GetInstance()->update();
	AnimationManagerC::GetInstance()->update(milliseconds);
	RenderingManagerC::GetInstance()->update();
}

void CGame::DrawScene(void)											
{
	startOpenGLDrawing();
	RenderingManagerC::GetInstance()->update();
}

CGame *CGame::CreateInstance()
{
	sInstance = new CGame();
	return sInstance;
}
void CGame::shutdown()
{
	PhysicsManagerC::GetInstance()->shutdown();
	InputManagerC::GetInstance()->shutdown();
	RenderingManagerC::GetInstance()->shutdown();
}
void CGame::DestroyGame(void)
{
	delete PhysicsManagerC::GetInstance();
	delete InputManagerC::GetInstance();
	delete RenderingManagerC::GetInstance();
	delete AnimationManagerC::GetInstance();
}