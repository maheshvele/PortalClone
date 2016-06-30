typedef enum LevelState
{
	LEVELSTATE_INVALID,
	LEVELSTATE_INIT_LOADLEVEL,
	LEVELSTATE_TRANSITION,
	LEVELSTATE_COMPLETE_LOADLEVEL,
	LEVELSTATE_MAX
}LevelState;

typedef struct Line
{ 
	Coord2D startPoint;
	Coord2D endPoint;
}Line;

typedef struct LevelDataStr
{
	char levelImage[FILENAME_MAX];
	std::list<Line> staticLevelLines;
	std::list<Line> dynamicLevelLines;
	std::list<Line> deathTriggers;
	std::list<Line> nonPortalLines;
	Coord2D enemy1;
	Coord2D enemy2;
	Coord2D	playerPosition;
	Line exitTrigger;

	bool isEnemy1Active;
	bool isEnemy2Active;

}LevelDataStr;


class LevelManagerC
{
public:
	static LevelManagerC	*CreateInstance();
	static LevelManagerC	*GetInstance() { return sInstance; };

	~LevelManagerC(){};

	void		init();

	void		update();

	void		parseLevelData();

	void		shutdown();

	void		levelFSM();
	
	LevelDataStr currentLevelData;
	
	void		loadLevelData(int level);

	void		destroyCurrentLevelData();
	
	int		GetCurrentState(){ return currentState; };

	void	SetCurrentState(int state) { currentState = state; };
	
	void	GoToNextLevel()
	{
		currentLevel++;
	}

private:
	int currentLevel;
	static LevelManagerC *sInstance;
	int currentState;
	LevelManagerC(){};
};