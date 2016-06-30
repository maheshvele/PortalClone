typedef struct portalStr
{
	bool isOrange;
	bool isActive;
	b2Vec2 normalDirection;
	b2Vec2 position;
	b2Body *portalBody;
}portalStr;

typedef enum EntityType{

	ENTITY_PLAYER,
	ENTITY_WALL,
	ENTITY_EXIT_TRIGGER,
	ENTITIY_ORANGE_PORTAL,
	ENTITY_BLUE_PORTAL,
	ENTITY_DEATH_TRIGGER,
	ENTITY_NON_PORTAL_WALL,
	ENTITY_ENEMY1,
	ENTITY_ENEMY2,
	ENTITY_ENEMY1_SENSOR,
	ENTITY_ENEMY2_SENSOR,
	ENTITY_BULLET,
	ENTITY_MAX
}EntityType;

class PhysicsObjectC
{
public:
	b2Vec2 GetPosition()
	{
		return mPhysicsBody->GetPosition();
	};
	b2Vec2 GetVelocity()
	{
		return mPhysicsBody->GetLinearVelocity();
	};

	void mTeleportBody(bool orangeToBlue, portalStr *bluePortal, portalStr *orangePortal);

	void SetPrePortalVelocity(b2Vec2 velocity)
	{
		mPrePortalVelocity.x = velocity.x;
		mPrePortalVelocity.y = velocity.y;
	};

	void SetTeleportFlag(bool isOrangeToBlue, bool isTeleport)
	{
		mIsOrangeToBlue = isOrangeToBlue;
		mIsTeleport = isTeleport;
	}

	void ApplyForceOnPlayer(b2Vec2 force)
	{
		mPhysicsBody->ApplyLinearImpulse(force, mPhysicsBody->GetPosition(), true);
	}

protected:
	b2Body	*mPhysicsBody;
	b2Vec2	mPrePortalVelocity;
	bool	mIsOrangeToBlue;
	bool	mIsTeleport;
};
