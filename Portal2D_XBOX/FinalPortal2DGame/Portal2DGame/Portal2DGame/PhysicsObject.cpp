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
#include <string>
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
		spawnLocation.x = (float)bluePortal->position.x +
			(float)bluePortal->normalDirection.x * PLAYER_OFFSET_X;
		spawnLocation.y = (float)bluePortal->position.y +
			(float)bluePortal->normalDirection.y * PLAYER_OFFSET_Y;

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
