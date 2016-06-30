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
#include "ShapeDraw.h"
#include "RenderingManager.h"
#include <list>
#include "PhysicsManager.h"
#include "InputManager.h"
#include "AnimationManager.h"

AnimationManagerC* AnimationManagerC::sInstance = NULL;


AnimationManagerC *AnimationManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new AnimationManagerC();
	return sInstance;
}
void AnimationManagerC::init()
{
	//Walk Cycle Init
	//Slice 14*
	WalkCycle[0].x = 2;
	WalkCycle[0].y = 270;
	WalkCycle[0].mWidth = 42;
	WalkCycle[0].mHeight = 78;
	WalkCycle[0].u1 = 0.0078125;
	WalkCycle[0].u2 = 0.171875;
	WalkCycle[0].v1 = 0.320313;
	WalkCycle[0].v2 = 0.472656;

	//Slice 15*
	WalkCycle[1].x = 50;
	WalkCycle[1].y = 107;
	WalkCycle[1].mWidth = 32;
	WalkCycle[1].mHeight = 79;
	WalkCycle[1].u1 = 0.195313;
	WalkCycle[1].u2 = 0.320313;
	WalkCycle[1].v1 = 0.636719;
	WalkCycle[1].v2 = 0.791016;

	//Slice 16*
	WalkCycle[2].x = 208;
	WalkCycle[2].y = 188;
	WalkCycle[2].mWidth = 39;
	WalkCycle[2].mHeight = 80;
	WalkCycle[2].u1 = 0.8125;
	WalkCycle[2].u2 = 0.964844;
	WalkCycle[2].v1 = 0.476563;
	WalkCycle[2].v2 = 0.632813;

	//Slice 17*
	WalkCycle[3].x = 193;
	WalkCycle[3].y = 270;
	WalkCycle[3].mWidth = 50;
	WalkCycle[3].mHeight = 78;
	WalkCycle[3].u1 = 0.753906;
	WalkCycle[3].u2 = 0.949219;
	WalkCycle[3].v1 = 0.320313;
	WalkCycle[3].v2 = 0.472656;

	//Slice 18*
	WalkCycle[4].x = 57;
	WalkCycle[4].y = 350;
	WalkCycle[4].mWidth = 54;
	WalkCycle[4].mHeight = 77;
	WalkCycle[4].u1 = 0.222656;
	WalkCycle[4].u2 = 0.433594;
	WalkCycle[4].v1 = 0.166016;
	WalkCycle[4].v2 = 0.316406;

	//Slice 19*
	WalkCycle[5].x = 125;
	WalkCycle[5].y = 188;
	WalkCycle[5].mWidth = 40;
	WalkCycle[5].mHeight = 78;
	WalkCycle[5].u1 = 0.488281;
	WalkCycle[5].u2 = 0.644531;
	WalkCycle[5].v1 = 0.480469;
	WalkCycle[5].v2 = 0.632813;

	//Slice 20*
	WalkCycle[6].x = 211;
	WalkCycle[6].y = 26;
	WalkCycle[6].mWidth = 29;
	WalkCycle[6].mHeight = 79;
	WalkCycle[6].u1 = 0.824219;
	WalkCycle[6].u2 = 0.9375;
	WalkCycle[6].v1 = 0.794922;
	WalkCycle[6].v2 = 0.949219;

	//Slice 21*
	WalkCycle[7].x = 84;
	WalkCycle[7].y = 188;
	WalkCycle[7].mWidth = 39;
	WalkCycle[7].mHeight = 79;
	WalkCycle[7].u1 = 0.328125;
	WalkCycle[7].u2 = 0.480469;
	WalkCycle[7].v1 = 0.478516;
	WalkCycle[7].v2 = 0.632813;

	//Slice 22*
	WalkCycle[8].x = 144;
	WalkCycle[8].y = 270;
	WalkCycle[8].mWidth = 47;
	WalkCycle[8].mHeight = 78;
	WalkCycle[8].u1 = 0.5625;
	WalkCycle[8].u2 = 0.746094;
	WalkCycle[8].v1 = 0.320313;
	WalkCycle[8].v2 = 0.472656;

	//Slice 23*
	WalkCycle[9].x = 2;
	WalkCycle[9].y = 350;
	WalkCycle[9].mWidth = 53;
	WalkCycle[9].mHeight = 77;
	WalkCycle[9].u1 = 0.0078125;
	WalkCycle[9].u2 = 0.214844;
	WalkCycle[9].v1 = 0.166016;
	WalkCycle[9].v2 = 0.316406;

	//Jump Init
	//Slice 24*
	Jump[0].x = 166;
	Jump[0].y = 107;
	Jump[0].mWidth = 40;
	Jump[0].mHeight = 76;
	Jump[0].u1 = 0.648438;
	Jump[0].u2 = 0.804688;
	Jump[0].v1 = 0.642578;
	Jump[0].v2 = 0.791016;

	//Slice 25*
	Jump[1].x = 2;
	Jump[1].y = 188;
	Jump[1].mWidth = 40;
	Jump[1].mHeight = 76;
	Jump[1].u1 = 0.0078125;
	Jump[1].u2 = 0.164063;
	Jump[1].v1 = 0.484375;
	Jump[1].v2 = 0.632813;

	//Slice 26*
	Jump[2].x = 208;
	Jump[2].y = 107;
	Jump[2].mWidth = 40;
	Jump[2].mHeight = 76;
	Jump[2].u1 = 0.8125;
	Jump[2].u2 = 0.96875;
	Jump[2].v1 = 0.642578;
	Jump[2].v2 = 0.791016;

	//Slice 27*
	Jump[3].x = 126;
	Jump[3].y = 107;
	Jump[3].mWidth = 38;
	Jump[3].mHeight = 78;
	Jump[3].u1 = 0.492188;
	Jump[3].u2 = 0.640625;
	Jump[3].v1 = 0.638672;
	Jump[3].v2 = 0.791016;

	//Slice 28*
	Jump[4].x = 44;
	Jump[4].y = 188;
	Jump[4].mWidth = 38;
	Jump[4].mHeight = 80;
	Jump[4].u1 = 0.171875;
	Jump[4].u2 = 0.320313;
	Jump[4].v1 = 0.476563;
	Jump[4].v2 = 0.632813;

	//Slice 29*
	Jump[5].x = 167;
	Jump[5].y = 188;
	Jump[5].mWidth = 39;
	Jump[5].mHeight = 80;
	Jump[5].u1 = 0.652344;
	Jump[5].u2 = 0.804688;
	Jump[5].v1 = 0.476563;
	Jump[5].v2 = 0.632813;

	//Idle 
	//Slice 1*
	Idle[0].x = 46;
	Idle[0].y = 270;
	Idle[0].mWidth = 47;
	Idle[0].mHeight = 77;
	Idle[0].u1 = 0.179688;
	Idle[0].u2 = 0.363281;
	Idle[0].v1 = 0.322266;
	Idle[0].v2 = 0.472656;

	//Slice 2*
	Idle[1].x = 95;
	Idle[1].y = 270;
	Idle[1].mWidth = 47;
	Idle[1].mHeight = 77;
	Idle[1].u1 = 0.371094;
	Idle[1].u2 = 0.554688;
	Idle[1].v1 = 0.322266;
	Idle[1].v2 = 0.472656;

	//Portals
	//Blue Portal - Slice 10*
	BluePortal.x = 70;
	BluePortal.y = 26;
	BluePortal.mWidth = 29;
	BluePortal.mHeight = 59;
	BluePortal.u1 = 0.273438;
	BluePortal.u2 = 0.386719;
	BluePortal.v1 = 0.833984;
	BluePortal.v2 = 0.949219;

	//Orange Portal - Slice 11*
	OrangePortal.x = 101;
	OrangePortal.y = 26;
	OrangePortal.mWidth = 29;
	OrangePortal.mHeight = 59;
	OrangePortal.u1 = 0.394531;
	OrangePortal.u2 = 0.507813;
	OrangePortal.v1 = 0.833984;
	OrangePortal.v2 = 0.949219;

	WalkSlice = 0;
	IdleSlice = 0;
	JumpSlice = 0;
}

void AnimationManagerC::update(DWORD milliseconds)
{
	mCurrentTime += milliseconds;

	MoveRight = InputManagerC::GetInstance()->isMoveRight;
	MoveLeft = InputManagerC::GetInstance()->isMoveLeft;
	Jumping = InputManagerC::GetInstance()->isJump;

	if (mCurrentTime - mLastUpdateTime > WALK_UPDATE_DELTA_TIME)
	{
		mLastUpdateTime = mCurrentTime;

		//If have time verify to play animation backwards

		if (Jumping == true)
		{
			CurrentSprite = Jump[JumpSlice];
			JumpSlice++;

			if (JumpSlice > 5)
			{
				JumpSlice = 0;
			}
		}

		else if (MoveRight == true || MoveLeft == true)
		{
			CurrentSprite = WalkCycle[WalkSlice];
			WalkSlice++;

			if (WalkSlice > 9)
			{
				WalkSlice = 0;
			}
		}

		else if (MoveRight == false && MoveLeft == false && Jumping == false)
		{
			if (IdleSlice == 0)
			{
				CurrentSprite = Idle[IdleSlice];
				IdleSlice = 1;
			}
			else
			{
				CurrentSprite = Idle[IdleSlice];
				IdleSlice = 0;
			}

			JumpSlice = 0;
		}
	}
}

void AnimationManagerC::shutdown()
{
}