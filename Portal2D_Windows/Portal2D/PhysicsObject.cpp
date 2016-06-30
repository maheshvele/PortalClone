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
#include "Box2D.h"
#include "MemoryManager.h"
#include "Box2D.h"
#include <list>
#include "PhysicsManager.h"
#include "InputManager.h"
#include "LevelManager.h"

float GetVelocityMagnitude(b2Vec2 velocity)
{
	return sqrt(velocity.x * velocity.x + velocity.y*velocity.y);
}

void PhysicsObjectC::mTeleportBody(bool orangeToBlue, portalStr *bluePortal, portalStr *orangePortal)
{
	b2Vec2 spawnLocation;
	b2Vec2 newVelocity;
	float prePortalSpeed = GetVelocityMagnitude(mPrePortalVelocity);

	if (!orangePortal->isActive || !bluePortal->isActive)
		return;

	if (orangeToBlue)
	{
		spawnLocation.x = bluePortal->position.x +
			bluePortal->normalDirection.x * PLAYER_OFFSET_X;
		spawnLocation.y = bluePortal->position.y +
			bluePortal->normalDirection.y * PLAYER_OFFSET_Y;

		newVelocity.x = prePortalSpeed * bluePortal->normalDirection.x;
		newVelocity.y = prePortalSpeed * bluePortal->normalDirection.y;

		this->mPhysicsBody->SetTransform(spawnLocation, 0.0);
		this->mPhysicsBody->SetLinearVelocity(newVelocity);
	}
	else
	{
		spawnLocation.x = orangePortal->position.x +
			orangePortal->normalDirection.x * PLAYER_OFFSET_X;
		spawnLocation.y = orangePortal->position.y +
			orangePortal->normalDirection.y * PLAYER_OFFSET_Y;

		newVelocity.x = prePortalSpeed * orangePortal->normalDirection.x;
		newVelocity.y = prePortalSpeed * orangePortal->normalDirection.y;

		this->mPhysicsBody->SetTransform(spawnLocation, 0.0);
		this->mPhysicsBody->SetLinearVelocity(newVelocity);
	}
}
