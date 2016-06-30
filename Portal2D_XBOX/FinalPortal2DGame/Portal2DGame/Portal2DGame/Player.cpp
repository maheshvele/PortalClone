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

void Player::initialize(b2Vec2 SpawnLocation, b2World *box2dWorld)
{
	b2Body *player;
	b2BodyDef playerDef;
	int entityType = ENTITY_PLAYER;

	SpawnLocation.x = PIXELS_TO_METERS * SpawnLocation.x;
	SpawnLocation.y = PIXELS_TO_METERS * SpawnLocation.y;

	playerDef.type = b2_dynamicBody;
	playerDef.position.Set(SpawnLocation.x, SpawnLocation.y);
	player = box2dWorld->CreateBody(&playerDef);

	b2PolygonShape playerShape;
	playerShape.SetAsBox(PLAYER_WIDHT / 2 * PIXELS_TO_METERS, PLAYER_HEIGHT / 2 * PIXELS_TO_METERS);

	b2FixtureDef playerFixtureDef;
	b2Fixture *playerFixture;
	playerFixtureDef.shape = &playerShape;
	playerFixture = player->CreateFixture(&playerFixtureDef);
	playerFixture->SetUserData((VOID *)entityType);

	mPhysicsBody = player;
}

void Player::update()
{
	
	InputManagerC *input = InputManagerC::GetInstance();
	b2Body *player = this->mPhysicsBody;

	if (input)
	{
		if (input->isMoveLeft)
		{
			player->ApplyForceToCenter(b2Vec2(-PLAYER_SPEED, 0), true);
		}

		if (input->isMoveRight)
		{
			player->ApplyForceToCenter(b2Vec2(PLAYER_SPEED, 0), true);
		}

		if (input->isJump)
		{
			if (player->GetLinearVelocity().y == 0.0f)
				player->ApplyLinearImpulse(b2Vec2(0, -PLAYER_JUMP), player->GetWorldCenter(), true);
		}
	}

	if (mIsTeleport)
	{
		this->mTeleportBody(mIsOrangeToBlue,
							PhysicsManagerC::GetInstance()->GetBluePortal(), 
							PhysicsManagerC::GetInstance()->GetOrangePortal());
	}
}

void Player::Respawn()
{
	this->mPhysicsBody->SetTransform(b2Vec2(LevelManagerC::GetInstance()->currentLevelData.playerPosition.x * PIXELS_TO_METERS,
		LevelManagerC::GetInstance()->currentLevelData.playerPosition.y * PIXELS_TO_METERS), 0.0);

}