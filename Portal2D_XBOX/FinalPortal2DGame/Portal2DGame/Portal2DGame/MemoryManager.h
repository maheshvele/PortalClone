class MemoryManagerC
{
public:
	static MemoryManagerC	*CreateInstance();
	static MemoryManagerC	*GetInstance() { return sInstance; };
	~MemoryManagerC(){};

	void		init();
	
	template<class T> T *MemAlloc(T type);
	template<class T> void MemFree(T *ptr);
	
	void		shutdown();
	void		PrintMemoryStatistics();
private:
	static MemoryManagerC *sInstance;

	int		mBytesAllocated;
	int		mBytesFreed;
	MemoryManagerC(){};
};