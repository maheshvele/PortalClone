#define SCREEN_SIZE_X 1370
#define SCREEN_SIZE_Y 770

class CGame
{
public:
	const char8_t *GetGameTitle(){return mGameTitle;}
	static CGame	*CreateInstance();
	static CGame	*GetInstance() {return sInstance;};
	~CGame();
	void DrawScene();
	void UpdateFrame(DWORD milliseconds);
	void DestroyGame();
	void init();
	void shutdown();
	static const uint32_t mScreenWidth = SCREEN_SIZE_X;
	static const uint32_t mScreenHeight = SCREEN_SIZE_Y;
	static const uint32_t mBitsPerPixel = 32;
private:
	static const char8_t mGameTitle[20];
	static CGame *sInstance;
	CGame(){};
};

