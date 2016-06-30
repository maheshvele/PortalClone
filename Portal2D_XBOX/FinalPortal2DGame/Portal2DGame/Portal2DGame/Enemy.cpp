#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include "baseTypes.h"
#include "gamedefs.h"
#include "Box2D.h"
#include "MemoryManager.h"
#include "Box2D.h"
#include <list>
#include "PhysicsManager.h"

#define ENEMY_WIDTH 40.0f
#define ENEMY_HEIGHT 50.0f

void Enemy::initialize(b2Vec2 SpawnLocation, b2World *box2dWorld, int entityType)
{
	b2Body *enemy;
	b2BodyDef enemyDef;

	SpawnLocation.x = PIXELS_TO_METERS * SpawnLocation.x;
	SpawnLocation.y = PIXELS_TO_METERS * SpawnLocation.y;

	enemyDef.type = b2_dynamicBody;
	enemyDef.position.Set(SpawnLocation.x, SpawnLocation.y);
	enemy = box2dWorld->CreateBody(&enemyDef);

	b2PolygonShape enemyShape;
	enemyShape.SetAsBox(ENEMY_WIDTH/ 2 * PIXELS_TO_METERS, ENEMY_HEIGHT/ 2 * PIXELS_TO_METERS);

	b2FixtureDef enemyFixtureDef;
	b2Fixture *enemyFixture;
	enemyFixtureDef.shape = &enemyShape;
	enemyFixture = enemy->CreateFixture(&enemyFixtureDef);
	enemyFixture->SetUserData((void *)entityType);


	//Add the attack sensor

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(300.0f /2.0f * PIXELS_TO_METERS , ENEMY_HEIGHT/2.0f * PIXELS_TO_METERS);

	b2FixtureDef sensorFixtureDef;
	b2Fixture *sensorFixture;

	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;
	sensorFixture = enemy->CreateFixture(&sensorFixtureDef);
	sensorFixture->SetUserData((void *)ENTITY_ENEMY1_SENSOR);
	
	mPhysicsBody = enemy;
}

void Enemy::update()
{
	if (!this->isActive)
	{
		return;
	}
	else
	{
		if (mIsTeleport)
		{
			this->mTeleportBody(mIsOrangeToBlue,
				PhysicsManagerC::GetInstance()->GetBluePortal(),
				PhysicsManagerC::GetInstance()->GetOrangePortal());
		}
	}
}

void Enemy::ShootAtPlayer()
{
	if (isFacingRight)
	{
		if ((PhysicsManagerC::GetInstance()->GetPlayerPosition().x) > (mPhysicsBody->GetPosition().x))
		{
			//Shoot at player
			PhysicsManagerC::GetInstance()->AddForceOnPlayer(b2Vec2(+100.0f, 0.0f));

		}
	}
	else
	{
		if ((PhysicsManagerC::GetInstance()->GetPlayerPosition().x) < (mPhysicsBody->GetPosition().x))
		{
			//Shoot at player
			PhysicsManagerC::GetInstance()->AddForceOnPlayer(b2Vec2(-100.0f, 0.0f));
		}
	}
}