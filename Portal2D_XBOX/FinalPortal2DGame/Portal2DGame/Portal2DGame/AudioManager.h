class AudioManagerC
{
public:
	static AudioManagerC *CreateInstance();
	static AudioManagerC *GetInstance() { return sInstance; };
	~AudioManagerC(){};
	void		init();
	void		update();
	void		shutdown();
private:
	static AudioManagerC *sInstance;
	AudioManagerC(){};
};