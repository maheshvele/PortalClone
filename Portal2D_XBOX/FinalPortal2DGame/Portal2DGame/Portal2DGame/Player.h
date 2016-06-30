class Player : public PhysicsObjectC
{
public:
	void initialize(b2Vec2 SpawnLocation, b2World *box2dWorld);
	void update();
	void Respawn();
	bool isTeleportPlayer;
	bool isRespawnPlayer;
};