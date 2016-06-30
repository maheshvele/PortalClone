#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations"
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include "baseTypes.h"
#include "Box2D.h"
#include "Box2D.h"
#include <list>
#include "InputManager.h"
#include "PhysicsManager.h"
#include "LevelManager.h"
PhysicsManagerC* PhysicsManagerC::sInstance = NULL;

PhysicsManagerC *PhysicsManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new PhysicsManagerC();
	return sInstance;
}

void PhysicsManagerC::init()
{
	sInstance->mBox2dWorld = new b2World(b2Vec2(0,GRAVITY));
	
	sInstance->player = new Player;

	sInstance->enemy1 = new Enemy;
	sInstance->enemy2 = new Enemy;

	sInstance->mBox2dWorld->SetContactListener(&sInstance->mCollisionListener);

	isRespawnPlayer = false;
}

void PhysicsManagerC::destroyCurrentLevel()
{
	b2Body *currentBody = mBox2dWorld->GetBodyList();
	b2Body *nextBody;

	while (currentBody)
	{
		nextBody = currentBody->GetNext();
		
		mBox2dWorld->DestroyBody(currentBody);

		currentBody = nextBody;
	}
}

void PhysicsManagerC::initCurrentLevel()
{
	// First Initialize the static objects in the level
	sInstance->mInitializeStaticObjects();

	//Level Platformm Details
	sInstance->mInitializeDynamicObjects();

	printf("Initializing Level");
}

void PhysicsManagerC::UpdatePortals()
{
	if (InputManagerC::GetInstance()->isOrangePortal ||
		InputManagerC::GetInstance()->isBluePortal)
	{
		sInstance->mRayCast();
	}

	if (InputManagerC::GetInstance()->isResetPortals)
	{
		sInstance->mOrangePortal.isActive = false;
		sInstance->mBluePortal.isActive = false;
	}
}

void PhysicsManagerC::update()
{
	int currentState = LevelManagerC::GetInstance()->GetCurrentState();
	
	if (currentState == LEVELSTATE_INIT_LOADLEVEL)
	{
		sInstance->initCurrentLevel();
	}
	
	sInstance->SetPlayerTeleportFlag(false, false);
	sInstance->SetEnemy1TeleportFlag(false, false);
	sInstance->SetEnemy2TeleportFlag(false, false);

	//Box2D Step Function 
	if (sInstance->mBox2dWorld)
	{
		sInstance->mBox2dWorld->Step(FRAMERATE, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
	}

	sInstance->UpdatePortals();

	currentState = LevelManagerC::GetInstance()->GetCurrentState();
	
	if (currentState == LEVELSTATE_TRANSITION)
	{
		sInstance->destroyCurrentLevel();
	}
	else
	{
		//Update the dynamic objects in the level
		sInstance->player->update();
		sInstance->enemy1->update();
		sInstance->enemy2->update();

		if (sInstance->isRespawnPlayer)
		{
			player->Respawn();
			sInstance->isRespawnPlayer = false;
		}
	}
}

void PhysicsManagerC::shutdown()
{
	//Destroy Box2D world
	if (sInstance->mBox2dWorld)
		delete sInstance->mBox2dWorld;

	delete player;
	delete enemy1;
	delete enemy2;
}

void PhysicsManagerC::mInitializeStaticObjects()
{
	//Level Borders
	b2Vec2 endPoint1, endPoint2;
	std::list<Line> staticObjectList = LevelManagerC::GetInstance()->currentLevelData.staticLevelLines;

	for (std::list<Line>::iterator it = staticObjectList.begin(); it != staticObjectList.end(); ++it)
	{
		endPoint1.x = (*it).startPoint.x;
		endPoint1.y = (*it).startPoint.y;

		endPoint2.x = (*it).endPoint.x;
		endPoint2.y = (*it).endPoint.y;

		mInitializeWall(endPoint1, endPoint2, ENTITY_WALL);
	}

	//Exit trigger
	endPoint1.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.x;
	endPoint1.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.y;

	endPoint2.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.x;
	endPoint2.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.y;
	
	mInitializeExitTrigger(endPoint1, endPoint2);

	//Non Portal Walls
	std::list<Line> staticNonPortalObjectList = LevelManagerC::GetInstance()->currentLevelData.nonPortalLines;
	
	for (std::list<Line>::iterator it = staticNonPortalObjectList.begin(); it != staticNonPortalObjectList.end(); ++it)
	{
		endPoint1.x = (*it).startPoint.x;
		endPoint1.y = (*it).startPoint.y;

		endPoint2.x = (*it).endPoint.x;
		endPoint2.y = (*it).endPoint.y;

		mInitializeWall(endPoint1, endPoint2, ENTITY_NON_PORTAL_WALL);
	}

	//Death Triggers
	std::list<Line> staticDeathObjectList = LevelManagerC::GetInstance()->currentLevelData.deathTriggers;

	for (std::list<Line>::iterator it = staticDeathObjectList.begin(); it != staticDeathObjectList.end(); ++it)
	{
		endPoint1.x = (*it).startPoint.x;
		endPoint1.y = (*it).startPoint.y;

		endPoint2.x = (*it).endPoint.x;
		endPoint2.y = (*it).endPoint.y;

		mInitializeWall(endPoint1, endPoint2, ENTITY_DEATH_TRIGGER);
	}

}

void PhysicsManagerC::mInitializeDynamicObjects()
{

	b2Vec2 playerLocation;

	playerLocation.x = LevelManagerC::GetInstance()->currentLevelData.playerPosition.x;
	playerLocation.y = LevelManagerC::GetInstance()->currentLevelData.playerPosition.y;

	b2Vec2 portalLocation;
	portalLocation.x = PORTAL_SPAWN_X;
	portalLocation.y = PORTAL_SPAWN_Y;

	sInstance->player->initialize(playerLocation, mBox2dWorld);
	//sInstance->mInitializePlayer(playerLocation);
	sInstance->mInitializePortal(portalLocation, true);
	sInstance->mInitializePortal(portalLocation, false);

	sInstance->mInitializeEnemies();
}

void PhysicsManagerC::mInitializeWall(b2Vec2 point1, b2Vec2 point2, int EntityType)
{
	b2Body *wall;
	b2BodyDef wallDef;

	b2Vec2 edge1, edge2;

	edge1.x = (point1.x )* PIXELS_TO_METERS;
	edge1.y = (point1.y +BOX2D_ERROR) * PIXELS_TO_METERS;

	edge2.x = (point2.x )* PIXELS_TO_METERS;
	edge2.y = (point2.y +BOX2D_ERROR)* PIXELS_TO_METERS;
	
	wallDef.type = b2_staticBody; // Body Type
	wall = sInstance->mBox2dWorld->CreateBody(&wallDef);

	b2EdgeShape wallShape;
	wallShape.Set(edge1, edge2);

	b2FixtureDef wallFixtureDef;
	b2Fixture *wallFixture;
	wallFixtureDef.shape = &wallShape;
	wallFixture = wall->CreateFixture(&wallFixtureDef);

	int entityType = EntityType;
	wallFixture->SetUserData((void *)EntityType);

}

void PhysicsManagerC::mInitializeExitTrigger(b2Vec2 point1, b2Vec2 point2)
{
	b2Body *wall;
	b2BodyDef wallDef;

	b2Vec2 edge1, edge2;

	edge1.x = (point1.x)* PIXELS_TO_METERS;
	edge1.y = (point1.y + BOX2D_ERROR) * PIXELS_TO_METERS;

	edge2.x = (point2.x)* PIXELS_TO_METERS;
	edge2.y = (point2.y + BOX2D_ERROR)* PIXELS_TO_METERS;

	wallDef.type = b2_staticBody; // Body Type
	wall = sInstance->mBox2dWorld->CreateBody(&wallDef);

	b2EdgeShape wallShape;
	wallShape.Set(edge1, edge2);

	b2FixtureDef wallFixtureDef;
	b2Fixture *wallFixture;
	wallFixtureDef.shape = &wallShape;
	wallFixture = wall->CreateFixture(&wallFixtureDef);

	int entityType = ENTITY_EXIT_TRIGGER;
	wallFixture->SetUserData((void *)ENTITY_EXIT_TRIGGER);

}

void PhysicsManagerC::mInitializePortal(b2Vec2 portalLocation, bool isOrange)
{
	b2Body *portalBody;
	b2BodyDef portalDef;

	int entityType;

	if (isOrange)
	{
		entityType = ENTITIY_ORANGE_PORTAL;
	}
	else
	{
		entityType = ENTITY_BLUE_PORTAL;
	}

	portalLocation.x = PIXELS_TO_METERS * portalLocation.x;
	portalLocation.y = PIXELS_TO_METERS * portalLocation.y;

	portalDef.type = b2_staticBody;
	portalDef.position.Set(portalLocation.x, portalLocation.y);
	portalBody = sInstance->mBox2dWorld->CreateBody(&portalDef);

	b2PolygonShape portalShape;
	portalShape.SetAsBox(PORTAL_WIDTH/ 2 * PIXELS_TO_METERS, PORTAL_HEIGHT/ 2 * PIXELS_TO_METERS);

	b2FixtureDef portalFixtureDef;
	b2Fixture	*portalFixture;
	portalFixtureDef.shape = &portalShape;
	portalFixture = portalBody->CreateFixture(&portalFixtureDef);
	portalFixture->SetUserData((void*)entityType);
	

	if (isOrange)
	{
		sInstance->mOrangePortal.portalBody = portalBody;
	}
	else
	{
		sInstance->mBluePortal.portalBody = portalBody;
	}

	sInstance->mOrangePortal.isActive = false;
	sInstance->mBluePortal.isActive = false;
}

void PhysicsManagerC::mInitializeEnemies()
{
	b2Vec2 enemy1Pos, enemy2Pos;

	enemy1Pos.x = LevelManagerC::GetInstance()->currentLevelData.enemy1.x;
	enemy1Pos.y = LevelManagerC::GetInstance()->currentLevelData.enemy1.y;
	
	if (LevelManagerC::GetInstance()->currentLevelData.isEnemy1Active == true)
	{
		enemy1->isActive = true;
		enemy1->initialize(enemy1Pos, mBox2dWorld, ENTITY_ENEMY1);
	}
	else
	{
		enemy1->isActive = false;
	}
	
	enemy2Pos.x = LevelManagerC::GetInstance()->currentLevelData.enemy2.x;
	enemy2Pos.y = LevelManagerC::GetInstance()->currentLevelData.enemy2.y;
	if (LevelManagerC::GetInstance()->currentLevelData.isEnemy2Active == true)
	{
		enemy2->isActive = true;
		//enemy2->initialize(enemy2Pos, mBox2dWorld, ENTITY_ENEMY2);
	}
	else
	{
		enemy2->isActive = false;
	}

	enemy1->isFacingRight = false;
}

void PhysicsManagerC::mRayCast()
{
	b2World *myWorld = sInstance->mBox2dWorld;
	MyRayCastListener *mRayCastListener = &(sInstance->mRayCastListener);
	b2Vec2 rayCastPosition;

	rayCastPosition.x = player->GetPosition().x + (PLAYER_RAYCAST_OFFSET_X - PLAYER_WIDHT/2) * PIXELS_TO_METERS;
	rayCastPosition.y = player->GetPosition().y + (PLAYER_RAYCAST_OFFSET_Y - PLAYER_HEIGHT/2) * PIXELS_TO_METERS;

	b2Vec2 mousePosition;

	mousePosition.x = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->x * PIXELS_TO_METERS;
	mousePosition.y = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->y * PIXELS_TO_METERS;

	myWorld->RayCast(mRayCastListener, rayCastPosition, mousePosition);
}

float32 MyRayCastListener::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	int userData;
	portalStr *portal;
	if (fixture)
	{
		userData = (int)fixture->GetUserData();

		if (fixture->IsSensor())
		{
			return 1.0f;
		}

		if (userData == ENTITY_WALL)
		{

			if (InputManagerC::GetInstance()->isOrangePortal)
			{

				//Initialize the orange portal at this location
				portal = PhysicsManagerC::GetInstance()->GetOrangePortal();

				portal->isActive = true;

				if (normal.y != 0)
				{
					portal->portalBody->SetTransform(point, 90.0f);
				}
				else
				{
					portal->portalBody->SetTransform(point, 0.0f);
				}

				portal->normalDirection.x = normal.x;
				portal->normalDirection.y = normal.y;

				portal->position.x = point.x;
				portal->position.y = point.y;
			}
			else if (InputManagerC::GetInstance()->isBluePortal)
			{

				//Initialize the blue portal at this location
				portal = PhysicsManagerC::GetInstance()->GetBluePortal();
				
				portal->isActive = true;

				if (normal.y != 0)
				{
					portal->portalBody->SetTransform(point, 90.0f);
				}
				else
				{
					portal->portalBody->SetTransform(point, 0.0f);
				}

				portal->normalDirection.x = normal.x;
				portal->normalDirection.y = normal.y;

				portal->position.x = point.x;
				portal->position.y = point.y;
			}
		}
	}

	return fraction;
}

void MyCollisionListener::BeginContact(b2Contact* contact)
{
	int bodyUserDataA = (int)contact->GetFixtureA()->GetUserData();
	int bodyUserDataB = (int)contact->GetFixtureB()->GetUserData();

	portalStr *orangePortal = PhysicsManagerC::GetInstance()->GetOrangePortal();
	portalStr *bluePortal = PhysicsManagerC::GetInstance()->GetBluePortal();
	b2Vec2 playerVelocity = PhysicsManagerC::GetInstance()->GetPlayerVelocity();
	b2Vec2 enemy1Velocity = PhysicsManagerC::GetInstance()->GetEnemy1Velocity();
	b2Vec2 enemy2Velocity = PhysicsManagerC::GetInstance()->GetEnemy2Velocity();

	if (bodyUserDataA == ENTITY_WALL || bodyUserDataB == ENTITY_WALL)
		return;

	if (bodyUserDataA == ENTITY_NON_PORTAL_WALL || bodyUserDataB == ENTITY_NON_PORTAL_WALL)
		return;

	if (bodyUserDataA == ENTITY_PLAYER)
	{
		if (bodyUserDataB == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(true, true);

			PhysicsManagerC::GetInstance()->SetPlayerPrePortalVelocity(playerVelocity);

		}
		else if (bodyUserDataB == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetPlayerPrePortalVelocity(playerVelocity);
		}
		else if (bodyUserDataB == ENTITY_EXIT_TRIGGER)
		{
			//Notify the level manager that the level has changed
			LevelManagerC::GetInstance()->SetCurrentState(LEVELSTATE_TRANSITION);
			LevelManagerC::GetInstance()->GoToNextLevel();
		}

		else if (bodyUserDataB == ENTITY_DEATH_TRIGGER)
		{
			PhysicsManagerC::GetInstance()->isRespawnPlayer = true;
		}

		else if (bodyUserDataB == ENTITY_BULLET)
		{
			PhysicsManagerC::GetInstance()->isKnockBackPlayer = true;
		}

		else if (bodyUserDataB == ENTITY_ENEMY1_SENSOR)
		{
			printf("Sensor Collision with player");
			PhysicsManagerC::GetInstance()->SetEnemy1ShootPlayer();
		}
		else if (bodyUserDataB == ENTITY_ENEMY2_SENSOR)
		{
			printf("Sensor Collision with player");
			PhysicsManagerC::GetInstance()->SetEnemy2ShootPlayer();
		}
	}
	else if (bodyUserDataB == ENTITY_PLAYER)
	{

		if (bodyUserDataA == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetPlayerPrePortalVelocity(playerVelocity);

		}
		else if (bodyUserDataA == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(true, true);
			PhysicsManagerC::GetInstance()->SetPlayerPrePortalVelocity(playerVelocity);
		}
		else if (bodyUserDataA == ENTITY_EXIT_TRIGGER)
		{
			//Notify the level manager that the level has 
			LevelManagerC::GetInstance()->SetCurrentState(LEVELSTATE_TRANSITION);
			LevelManagerC::GetInstance()->GoToNextLevel();
		}

		else if (bodyUserDataA == ENTITY_DEATH_TRIGGER)
		{
			PhysicsManagerC::GetInstance()->isRespawnPlayer = true;
		}
		else if (bodyUserDataA == ENTITY_BULLET)
		{
			PhysicsManagerC::GetInstance()->isKnockBackPlayer = true;
		}
		else if (bodyUserDataA == ENTITY_ENEMY1_SENSOR)
		{
			printf("Sensor Collision with player");
			PhysicsManagerC::GetInstance()->SetEnemy1ShootPlayer();
		}
		else if (bodyUserDataA == ENTITY_ENEMY2_SENSOR)
		{
			printf("Sensor Collision with player");
			PhysicsManagerC::GetInstance()->SetEnemy2ShootPlayer();
		}
	}

	else if (bodyUserDataA == ENTITY_ENEMY1)
	{
		if (bodyUserDataB == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetEnemy1TeleportFlag(true, true);
			PhysicsManagerC::GetInstance()->SetEnemy1PrePortalVelocity(enemy1Velocity);

		}
		else if (bodyUserDataB == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetEnemy1TeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetEnemy1PrePortalVelocity(enemy1Velocity);
		}
	}

	else if (bodyUserDataB == ENTITY_ENEMY1)
	{
		if (bodyUserDataA == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetEnemy1TeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetEnemy1PrePortalVelocity(enemy1Velocity);

		}
		else if (bodyUserDataA == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetEnemy1TeleportFlag(true, true);
			PhysicsManagerC::GetInstance()->SetEnemy1PrePortalVelocity(enemy1Velocity);
		}
	}

	else if (bodyUserDataA == ENTITY_ENEMY2)
	{
		if (bodyUserDataB == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(true, true);
			PhysicsManagerC::GetInstance()->SetEnemy2PrePortalVelocity(enemy2Velocity);

		}

		else if (bodyUserDataB == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetEnemy2PrePortalVelocity(enemy2Velocity);
		}
	}
	else if (bodyUserDataB == ENTITY_ENEMY2)
	{
		if (bodyUserDataA == ENTITY_BLUE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(false, true);
			PhysicsManagerC::GetInstance()->SetEnemy2PrePortalVelocity(enemy2Velocity);

		}
		else if (bodyUserDataA == ENTITIY_ORANGE_PORTAL)
		{
			PhysicsManagerC::GetInstance()->SetPlayerTeleportFlag(true, true);
			PhysicsManagerC::GetInstance()->SetEnemy2PrePortalVelocity(enemy2Velocity);
		}
	}
}
