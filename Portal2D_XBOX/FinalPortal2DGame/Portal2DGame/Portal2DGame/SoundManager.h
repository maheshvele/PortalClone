

class SoundManagerC
{
public:
	static SoundManagerC	*CreateInstance();
	static SoundManagerC	*GetInstance() { return sInstance; };

	~SoundManagerC(){};

	void		init();
	void		update();
	void		shutdown();

	//HRESULT                 hr;						// Return code
	IXACTEngine*            pXACT;					// XACT Engine instance
	IXACTWaveBank*          pStreamingWaveBank;      // XACT Wave Bank
	IXACTSoundBank*         pSoundBank;				// XACT Sound Bank
	IXACTSoundSource*       pSoundSource;			 // XACT Sound Source
	IXACTSoundSource*       pSoundSourceSFX;         // XACT Sound Source
	IXACTSoundSource*       pSoundSourceFoot;         // XACT Sound Source


	BYTE* pbSoundBank;

private:
	static SoundManagerC *sInstance;
	SoundManagerC(){};
};