class Enemy : public PhysicsObjectC
{

public:
	void initialize(b2Vec2 SpawnLocation, b2World *box2dWorld, int entityType);
	void update();
	void FindPlayer();
	void ShootAtPlayer();
	bool isActive;
	bool isFacingRight;
	bool isFoundInSensor;
};