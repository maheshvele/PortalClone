typedef struct Sprite
{
	int x, y, mWidth, mHeight;
	float u1, u2, v1, v2;

}Sprite;

class AnimationManagerC
{
public:
	static AnimationManagerC	*CreateInstance();
	static AnimationManagerC	*GetInstance() { return sInstance; };
	~AnimationManagerC(){};
	void		init();
	void		update(DWORD milliseconds);
	void		shutdown();

	Sprite WalkCycle[10]; // slice 14 to 23
	Sprite Jump[6]; // slice 24 to 29
	Sprite Idle[2]; // slice 1 and 2
	Sprite OrangePortal;
	Sprite BluePortal;

	Sprite CurrentSprite;

	DWORD mLastUpdateTime;
	DWORD mCurrentTime;

	int WalkSlice, IdleSlice, JumpSlice;

	bool MoveRight, MoveLeft, Jumping;

	//GLuint  mPlayerTexture;

private:
	static AnimationManagerC *sInstance;
	AnimationManagerC(){};
};

#define WALK_UPDATE_DELTA_TIME 50 