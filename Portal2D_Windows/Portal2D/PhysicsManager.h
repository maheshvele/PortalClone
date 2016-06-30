#include "PhysicsObject.h"
#include "Player.h"
#include "Enemy.h"

class MyCollisionListener :public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact){};
};

class MyRayCastListener :public b2RayCastCallback
{
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
};

class PhysicsManagerC
{
public:
	static PhysicsManagerC	*CreateInstance();
	static PhysicsManagerC	*GetInstance() { return sInstance; };
	~PhysicsManagerC(){};
	void		init();
	void		initCurrentLevel(); //Initialize static and dynamic bodies in the level
	void		destroyCurrentLevel(); //Destroy current Level's box2D data
	void		update();			// call the box2D step function here
	void		cleanUpCurrentLevel(); //Remove the box2D objects that we created
	void		shutdown();
	bool		isTeleportPlayer;	//Can the player teleport?
	bool		isTeleportEnemy1;
	bool		isTeleportEnemy2;
	bool		isTeleportEnemy3;
	bool		isOrangeToBlue;	    // if isTeleportPlayer is set this flag checks the direction
	// of portals;
	bool		isRespawnPlayer;    //respawns player if it dies
	bool		isKnockBackPlayer;  //knocks back the player

	//b2Vec2	GetPlayerPosition() { return mPlayer->GetPosition(); }; //Get Current player position

	portalStr *GetOrangePortal() { return &sInstance->mOrangePortal; }; //Get Orange Portal details

	portalStr *GetBluePortal() { return &sInstance->mBluePortal; }; //Get Blue Portal details

	b2Vec2  GetPlayerVelocity(){ return player->GetVelocity(); }; // Get the player velocity
	b2Vec2  GetEnemy1Velocity()
	{
		if (enemy1->isActive)
			return enemy1->GetVelocity();
		else
			return b2Vec2(0.0f, 0.0f);
	};
	b2Vec2  GetEnemy2Velocity()
	{
		if (enemy1->isActive)
			return enemy1->GetVelocity();
		else
			return b2Vec2(0.0f, 0.0f);
	};

	b2Vec2 GetPlayerPosition() { return player->GetPosition(); };
	b2Vec2 GetEnemy1Position() { return enemy1->GetPosition(); };
	b2Vec2 GetEnemy2Position() { return enemy2->GetPosition(); };

	//Set the preportal velocity variable
	void SetPlayerPrePortalVelocity(b2Vec2 vel)
	{
		player->SetPrePortalVelocity(vel);
	};

	void SetEnemy1PrePortalVelocity(b2Vec2 vel)
	{
		enemy1->SetPrePortalVelocity(vel);
	};

	void SetEnemy2PrePortalVelocity(b2Vec2 vel)
	{
		enemy2->SetPrePortalVelocity(vel);
	};

	void SetPlayerTeleportFlag(bool isOrangeToBlue, bool isTeleport)
	{
		player->SetTeleportFlag(isOrangeToBlue, isTeleport);
	};

	void SetEnemy1TeleportFlag(bool isOrangeToBlue, bool isTeleport)
	{
		enemy1->SetTeleportFlag(isOrangeToBlue, isTeleport);
	};

	void SetEnemy2TeleportFlag(bool isOrangeToBlue, bool isTeleport)
	{
		enemy2->SetTeleportFlag(isOrangeToBlue, isTeleport);
	};

	portalStr *GetOrangePortalData()
	{
		return &mOrangePortal;
	};

	portalStr *GetBluePortalData()
	{
		return &mBluePortal;
	};

	void SetEnemy1ShootPlayer()
	{
		enemy1->ShootAtPlayer();
	};

	void SetEnemy2ShootPlayer()
	{
		enemy2->ShootAtPlayer();
	};

	void AddForceOnPlayer(b2Vec2 force)
	{
		player->ApplyForceOnPlayer(force);
	}

private:
	
	static PhysicsManagerC *sInstance;
		
	//Box2D variables
	b2World *mBox2dWorld;
	MyCollisionListener mCollisionListener;
	MyRayCastListener	mRayCastListener;
	
	portalStr mOrangePortal;	//orange portal data
	portalStr mBluePortal;		//blue portal data
	PhysicsManagerC(){};

	//Member Methods
	void mInitializeStaticObjects();		//Initialize static objects like walls and platforms on the level.
	void mInitializeDynamicObjects();		//Initialize dynamic objects 
	void mInitializeWall(b2Vec2 point1, b2Vec2 point2, int EntityType);
	void mInitializeExitTrigger(b2Vec2 point1, b2Vec2 point2);
	void mInitializeEnemies();
	void mInitializePortal(b2Vec2 portalLocation, bool isOrange);
	void mRayCast();
	void UpdatePortals();
	void UpdatePlayer();
	void UpdateEnemies();

	Player *player;
	Enemy *enemy1;
	Enemy *enemy2;
};


#define METERS_TO_PIXELS 100.0
#define PIXELS_TO_METERS 0.01
#define GRAVITY 5.0f
#define FRAMERATE 1.0/60.0
#define VELOCITY_ITERATIONS 5
#define POSITION_ITERATIONS 5
#define BOX2D_ERROR 0.0

#define PLAYER_SPAWN_X 600.0
#define PLAYER_SPAWN_Y 500.0

#define PLAYER_WIDHT  47.0f
#define PLAYER_HEIGHT 77.0f

#define ENEMY_WIDTH 40.0f
#define ENEMY_HEIGHT 50.0f

#define PLAYER_SPEED 5.0f
#define PLAYER_JUMP	 1.0f

#define PORTAL_SPAWN_X 0.0
#define PORTAL_SPAWN_Y 0.0
#define PORTAL_WIDTH (5.0)
#define PORTAL_HEIGHT (PLAYER_HEIGHT + 10)

#define PLAYER_OFFSET_X (PLAYER_WIDHT * PIXELS_TO_METERS /2 + 0.18f)
#define PLAYER_OFFSET_Y (PLAYER_HEIGHT * PIXELS_TO_METERS /2+ 0.18f)

#define ENEMY_OFFSET_X (ENEMY_WIDTH * PIXELS_TO_METERS/2 + 0.18f)
#define ENEMY_OFFSET_Y (ENEMY_HEIGHT * PIXELS_TO_METERS/2 + 0.18f)

#define PLAYER_RAYCAST_OFFSET_X		15.0
#define PLAYER_RAYCAST_OFFSET_Y		15.0

#define RAD_TO_DEGREES	57.2957795f

