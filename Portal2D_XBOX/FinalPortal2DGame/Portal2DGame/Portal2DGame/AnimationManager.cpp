#define RENDERING_MANAGER_CPP
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include "baseTypes.h"	
#include  "game.h"
#include "gamedefs.h"
#include "SOIL.h"
#include "Box2D.h"
#include "ShapeDraw.h"
#include "RenderingManager.h"
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
	WalkCycle[0].u1 = 0.0078125f;
	WalkCycle[0].u2 = 0.171875f;
	WalkCycle[0].v1 = 0.320313f;
	WalkCycle[0].v2 = 0.472656f;

	WalkRect[0].left = 2;
	WalkRect[0].right = 44;
	WalkRect[0].top = 270;
	WalkRect[0].bottom = 348;

	//Slice 15*
	WalkCycle[1].x = 50;
	WalkCycle[1].y = 107;
	WalkCycle[1].mWidth = 32;
	WalkCycle[1].mHeight = 79;
	WalkCycle[1].u1 = 0.195313f;
	WalkCycle[1].u2 = 0.320313f;
	WalkCycle[1].v1 = 0.636719f;
	WalkCycle[1].v2 = 0.791016f;

	WalkRect[1].left = 50;
	WalkRect[1].right = 82;
	WalkRect[1].top = 107;
	WalkRect[1].bottom = 186;

	//Slice 16*
	WalkCycle[2].x = 208;
	WalkCycle[2].y = 188;
	WalkCycle[2].mWidth = 39;
	WalkCycle[2].mHeight = 80;
	WalkCycle[2].u1 = 0.8125f;
	WalkCycle[2].u2 = 0.964844f;
	WalkCycle[2].v1 = 0.476563f;
	WalkCycle[2].v2 = 0.632813f;

	WalkRect[2].left = 208;
	WalkRect[2].right = 247;
	WalkRect[2].top = 188;
	WalkRect[2].bottom = 268;

	//Slice 17*
	WalkCycle[3].x = 193;
	WalkCycle[3].y = 270;
	WalkCycle[3].mWidth = 50;
	WalkCycle[3].mHeight = 78;
	WalkCycle[3].u1 = 0.753906f;
	WalkCycle[3].u2 = 0.949219f;
	WalkCycle[3].v1 = 0.320313f;
	WalkCycle[3].v2 = 0.472656f;

	WalkRect[3].left = 193;
	WalkRect[3].right = 243;
	WalkRect[3].top = 270;
	WalkRect[3].bottom = 348;

	//Slice 18*
	WalkCycle[4].x = 57;
	WalkCycle[4].y = 350;
	WalkCycle[4].mWidth = 54;
	WalkCycle[4].mHeight = 77;
	WalkCycle[4].u1 = 0.222656f;
	WalkCycle[4].u2 = 0.433594f;
	WalkCycle[4].v1 = 0.166016f;
	WalkCycle[4].v2 = 0.316406f;

	WalkRect[4].left = 57;
	WalkRect[4].right = 111;
	WalkRect[4].top = 350;
	WalkRect[4].bottom = 427;

	//Slice 19*
	WalkCycle[5].x = 125;
	WalkCycle[5].y = 188;
	WalkCycle[5].mWidth = 40;
	WalkCycle[5].mHeight = 78;
	WalkCycle[5].u1 = 0.488281f;
	WalkCycle[5].u2 = 0.644531f;
	WalkCycle[5].v1 = 0.480469f;
	WalkCycle[5].v2 = 0.632813f;

	WalkRect[5].left = 125;
	WalkRect[5].right = 165;
	WalkRect[5].top = 188;
	WalkRect[5].bottom = 266;

	//Slice 20*
	WalkCycle[6].x = 211;
	WalkCycle[6].y = 26;
	WalkCycle[6].mWidth = 29;
	WalkCycle[6].mHeight = 79;
	WalkCycle[6].u1 = 0.824219f;
	WalkCycle[6].u2 = 0.9375f;
	WalkCycle[6].v1 = 0.794922f;
	WalkCycle[6].v2 = 0.949219f;

	WalkRect[6].left = 211;
	WalkRect[6].right = 240;
	WalkRect[6].top = 26;
	WalkRect[6].bottom = 105;

	//Slice 21*
	WalkCycle[7].x = 84;
	WalkCycle[7].y = 188;
	WalkCycle[7].mWidth = 39;
	WalkCycle[7].mHeight = 79;
	WalkCycle[7].u1 = 0.328125f;
	WalkCycle[7].u2 = 0.480469f;
	WalkCycle[7].v1 = 0.478516f;
	WalkCycle[7].v2 = 0.632813f;

	WalkRect[7].left = 84;
	WalkRect[7].right = 123;
	WalkRect[7].top = 188;
	WalkRect[7].bottom = 267;

	//Slice 22*
	WalkCycle[8].x = 144;
	WalkCycle[8].y = 270;
	WalkCycle[8].mWidth = 47;
	WalkCycle[8].mHeight = 78;
	WalkCycle[8].u1 = 0.5625f;
	WalkCycle[8].u2 = 0.746094f;
	WalkCycle[8].v1 = 0.320313f;
	WalkCycle[8].v2 = 0.472656f;

	WalkRect[8].left = 144;
	WalkRect[8].right = 191;
	WalkRect[8].top = 270;
	WalkRect[8].bottom = 348;

	//Slice 23*
	WalkCycle[9].x = 2;
	WalkCycle[9].y = 350;
	WalkCycle[9].mWidth = 53;
	WalkCycle[9].mHeight = 77;
	WalkCycle[9].u1 = 0.0078125f;
	WalkCycle[9].u2 = 0.214844f;
	WalkCycle[9].v1 = 0.166016f;
	WalkCycle[9].v2 = 0.316406f;

	WalkRect[9].left = 2;
	WalkRect[9].right = 55;
	WalkRect[9].top = 350;
	WalkRect[9].bottom = 427;

	//Jump Init
	//Slice 24*
	Jump[0].x = 166;
	Jump[0].y = 107;
	Jump[0].mWidth = 40;
	Jump[0].mHeight = 76;
	Jump[0].u1 = 0.648438f;
	Jump[0].u2 = 0.804688f;
	Jump[0].v1 = 0.642578f;
	Jump[0].v2 = 0.791016f;

	JumpRect[0].left = 166;
	JumpRect[0].right = 206;
	JumpRect[0].top = 107;
	JumpRect[0].bottom = 183;

	//Slice 25*
	Jump[1].x = 2;
	Jump[1].y = 188;
	Jump[1].mWidth = 40;
	Jump[1].mHeight = 76;
	Jump[1].u1 = 0.0078125f;
	Jump[1].u2 = 0.164063f;
	Jump[1].v1 = 0.484375f;
	Jump[1].v2 = 0.632813f;

	JumpRect[1].left = 2;
	JumpRect[1].right = 42;
	JumpRect[1].top = 188;
	JumpRect[1].bottom = 264;

	//Slice 26*
	Jump[2].x = 208;
	Jump[2].y = 107;
	Jump[2].mWidth = 40;
	Jump[2].mHeight = 76;
	Jump[2].u1 = 0.8125f;
	Jump[2].u2 = 0.96875f;
	Jump[2].v1 = 0.642578f;
	Jump[2].v2 = 0.791016f;

	JumpRect[2].left = 208;
	JumpRect[2].right = 248;
	JumpRect[2].top = 107;
	JumpRect[2].bottom = 183;

	//Slice 27*
	Jump[3].x = 126;
	Jump[3].y = 107;
	Jump[3].mWidth = 38;
	Jump[3].mHeight = 78;
	Jump[3].u1 = 0.492188f;
	Jump[3].u2 = 0.640625f;
	Jump[3].v1 = 0.638672f;
	Jump[3].v2 = 0.791016f;

	JumpRect[3].left = 126;
	JumpRect[3].right = 164;
	JumpRect[3].top = 107;
	JumpRect[3].bottom = 185;

	//Slice 28*
	Jump[4].x = 44;
	Jump[4].y = 188;
	Jump[4].mWidth = 38;
	Jump[4].mHeight = 80;
	Jump[4].u1 = 0.171875f;
	Jump[4].u2 = 0.320313f;
	Jump[4].v1 = 0.476563f;
	Jump[4].v2 = 0.632813f;

	JumpRect[4].left = 44;
	JumpRect[4].right = 82;
	JumpRect[4].top = 188;
	JumpRect[4].bottom = 268;

	//Slice 29*
	Jump[5].x = 167;
	Jump[5].y = 188;
	Jump[5].mWidth = 39;
	Jump[5].mHeight = 80;
	Jump[5].u1 = 0.652344f;
	Jump[5].u2 = 0.804688f;
	Jump[5].v1 = 0.476563f;
	Jump[5].v2 = 0.632813f;

	JumpRect[5].left = 167;
	JumpRect[5].right = 206;
	JumpRect[5].top = 188;
	JumpRect[5].bottom = 268;

	//Idle 
	//Slice 1*
	Idle[0].x = 46;
	Idle[0].y = 270;
	Idle[0].mWidth = 47;
	Idle[0].mHeight = 77;
	Idle[0].u1 = 0.179688f;
	Idle[0].u2 = 0.363281f;
	Idle[0].v1 = 0.322266f;
	Idle[0].v2 = 0.472656f;

	IdleRect[0].left = 46;
	IdleRect[0].right = 93;
	IdleRect[0].top = 270;
	IdleRect[0].bottom = 347;

	//Slice 2*
	Idle[1].x = 95;
	Idle[1].y = 270;
	Idle[1].mWidth = 47;
	Idle[1].mHeight = 77;
	Idle[1].u1 = 0.371094f;
	Idle[1].u2 = 0.554688f;
	Idle[1].v1 = 0.322266f;
	Idle[1].v2 = 0.472656f;

	IdleRect[1].left = 95;
	IdleRect[1].right = 142;
	IdleRect[1].top = 270;
	IdleRect[1].bottom = 347;

	//Portals
	//Blue Portal - Slice 10*
	BluePortal.x = 70;
	BluePortal.y = 26;
	BluePortal.mWidth = 29;
	BluePortal.mHeight = 59;
	BluePortal.u1 = 0.273438f;
	BluePortal.u2 = 0.386719f;
	BluePortal.v1 = 0.833984f;
	BluePortal.v2 = 0.949219f;

	BlueRect.left = 70;
	BlueRect.right = 99;
	BlueRect.top = 26;
	BlueRect.bottom = 85;

	//Orange Portal - Slice 11*
	OrangePortal.x = 101;
	OrangePortal.y = 26;
	OrangePortal.mWidth = 29;
	OrangePortal.mHeight = 59;
	OrangePortal.u1 = 0.394531f;
	OrangePortal.u2 = 0.507813f;
	OrangePortal.v1 = 0.833984f;
	OrangePortal.v2 = 0.949219f;

	OrangeRect.left = 101;
	OrangeRect.right = 130;
	OrangeRect.top = 26;
	OrangeRect.bottom = 85;

	PlayerHand.left = 183;
	PlayerHand.right = 238;
	PlayerHand.top = 2;
	PlayerHand.bottom = 22;

	WalkSlice = 0;
	IdleSlice = 0;
	JumpSlice = 0;


	//WalkRect.right = 44.0;
	//WalkRect.left = 2.0;
	//WalkRect.top = 270.0;
	//WalkRect.bottom = 348.0;

	mCurrentTime = 0;
	mLastUpdateTime = 0;

}

//void AnimationManagerC::update(DWORD milliseconds)
void AnimationManagerC::update(float milliseconds)
{

	//CurrentSpriteRect = WalkRect[5];

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
			//CurrentSprite = Jump[JumpSlice];
			CurrentSpriteRect = JumpRect[JumpSlice];
			JumpSlice++;

			if (JumpSlice > 5)
			{
				JumpSlice = 0;
			}
		}

		else if (MoveRight == true || MoveLeft == true)
		{
			//CurrentSprite = WalkCycle[WalkSlice];

			CurrentSpriteRect = WalkRect[WalkSlice];
			WalkSlice++;

			if (WalkSlice > 9)
			{
				WalkSlice = 0;
			}
		}

		else if (MoveRight == false && MoveLeft == false && Jumping == false)
		{
			//CurrentSpriteRect = WalkRect[0];

			if (IdleSlice == 0)
			{
				//CurrentSprite = Idle[IdleSlice];
				CurrentSpriteRect = IdleRect[IdleSlice];
				IdleSlice = 1;
			}
			else
			{
				//CurrentSprite = Idle[IdleSlice];
				CurrentSpriteRect = IdleRect[IdleSlice];
				IdleSlice = 0;
			}

			JumpSlice = 0;
		}
	}
}

void AnimationManagerC::shutdown()
{
}