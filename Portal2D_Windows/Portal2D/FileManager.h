class FileManagerC
{
public:
	static FileManagerC	*CreateInstance();
	static FileManagerC	*GetInstance() { return sInstance; };
	~FileManagerC(){};
	void		init();
	void		update();

	GLuint		ReadTextureFile();
	void		ReadLevelDataFile();
	void		ReadAudioFile();
	void		OpenDebugFile();

	void		shutdown();
private:
	static FileManagerC *sInstance;
	FileManagerC(){};

	void mOpenFile(const char *fileName, int fileOperation);
};

typedef enum FileOperation
{
	OPEN_TEXT_FILE,
	OPEN_DEBUG_FILE,
	OPEN_TEXTURE_FILE,
	OPEN_AUDIO_FILE,
	OPEN_LEVEL_FILE

}FileOperation;