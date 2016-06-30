class RenderingManagerC
{
public:
	static RenderingManagerC	*CreateInstance();
	static RenderingManagerC	*GetInstance() { return sInstance; };
	~RenderingManagerC(){};
	void		init();
	void		update();
	void		shutdown();
private:
	static RenderingManagerC *sInstance;
	RenderingManagerC(){};
	GLuint	mBackgroundTexture;
	GLuint  mPlayerTexture;
};