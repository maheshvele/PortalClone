#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>	// Header File For Variable Argument Routines
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include <xact.h>
#include <dsstdfx.h>
#include <math.h>												// Header File For Math Operations
#include "baseTypes.h"	
#include  "game.h"
#include "gamedefs.h"
#include "SOIL.h"
#include "Box2D.h"
#include "ShapeDraw.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "AnimationManager.h"
#include "SoundManager.h"
#include <xact.h>
#include <dsstdfx.h>

SoundManagerC* SoundManagerC::sInstance = NULL;


SoundManagerC *SoundManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new SoundManagerC();
	return sInstance;
}

void SoundManagerC::init()
{
	// Initialize the XACT runtime parameters
	XACT_RUNTIME_PARAMETERS xrParams = { 0 };
	xrParams.dwMax2DHwVoices = 128; // Maximum number of 2D hardware voices/streams XACT engine will use
	xrParams.dwMax3DHwVoices = 32;  // Maximum number of 3D hardware voices/streams XACT engine will use, between 0 and 64
	xrParams.dwMaxConcurrentStreams = 16;  // Maximum number of 2D voices XACT may use for streaming from the hard drive/DVD
	xrParams.dwMaxNotifications = 0;   // Maximum number of notifications, 0 = default

	XACTEngineCreate(&xrParams, &pXACT);


	// Open the streaming wave bank file
	HANDLE hStreamingWaveBank = CreateFile("D:\\media\\PortalSoundBank.xwb", //***
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

	// Initialize the streaming wave bank parameters
	XACT_WAVEBANK_STREAMING_PARAMETERS wbParams = { 0 };
	wbParams.hFile = hStreamingWaveBank;
	wbParams.dwOffset = 0;
	wbParams.dwPacketSizeInMilliSecs = 400;
	wbParams.dwPrimePacketSizeInMilliSecs = 400;

	pXACT->RegisterStreamedWaveBank(&wbParams, &pStreamingWaveBank);
		
	pbSoundBank = NULL;
	DWORD dwFileSize = 0;

	XBUtil_LoadFile("D:\\media\\SFXBank.xsb", (VOID **)&pbSoundBank, &dwFileSize);
	
	pXACT->CreateSoundBank(pbSoundBank, dwFileSize, &pSoundBank);


	// Download the standard DirectSound effects image
	DSEFFECTIMAGELOC EffectLoc;
	EffectLoc.dwI3DL2ReverbIndex = GraphI3DL2_I3DL2Reverb;
	EffectLoc.dwCrosstalkIndex = GraphXTalk_XTalk;

	XAudioDownloadEffectsImage("d:\\media\\dsstdfx.bin",
		&EffectLoc,
		XAUDIO_DOWNLOADFX_EXTERNFILE,
		NULL);

	// Create a 3D sound source to play our cues on
	HRESULT hr=pXACT->CreateSoundSource(XACT_FLAG_SOUNDSOURCE_2D, &pSoundSource);
		

	// Get the sound cue index from the sound bank
	DWORD dwSoundCueIndex = 0;

	pSoundBank->GetSoundCueIndexFromFriendlyName("background",             // Null-terminated string representing the friendly
		&dwSoundCueIndex);   // Pointer to the returned SoundCue index for friendly name

	hr=pXACT->CreateSoundSource(XACT_FLAG_SOUNDSOURCE_2D, &pSoundSourceSFX);

	// Get the sound cue index from the sound bank
	DWORD dwSoundCueIndexSFX = 6;


	pSoundBank->GetSoundCueIndexFromFriendlyName("Hello",             // Null-terminated string representing the friendly
		&dwSoundCueIndexSFX);   // Pointer to the returned SoundCue index for friendly name

	hr=pXACT->CreateSoundSource(XACT_FLAG_SOUNDSOURCE_2D, &pSoundSourceFoot);
	DWORD dwSoundCueIndexFoot = 8;
	pSoundBank->GetSoundCueIndexFromFriendlyName("IdontHateYou",             // Null-terminated string representing the friendly
		&dwSoundCueIndexFoot); 

	pSoundBank->GetSoundCueIndexFromFriendlyName("Steps",             // Null-terminated string representing the friendly
		&dwSoundCueIndexFoot); 


	// Initialize XACT notification struct
	XACT_NOTIFICATION_DESCRIPTION xactNotificationDesc = { 0 };
	xactNotificationDesc.wType = eXACTNotification_Stop;
	xactNotificationDesc.wFlags = XACT_FLAG_NOTIFICATION_USE_SOUNDCUE_INDEX;
	xactNotificationDesc.u.pSoundBank = pSoundBank;
	xactNotificationDesc.dwSoundCueIndex = dwSoundCueIndex;
	xactNotificationDesc.hEvent = NULL;

	pXACT->RegisterNotification(&xactNotificationDesc);
		

	// Play the sound cue
	pSoundBank->Play(dwSoundCueIndex, pSoundSource, XACT_FLAG_SOUNDCUE_AUTORELEASE, NULL);

}

void SoundManagerC::update()
{
	bool IsWalking = false, CreateStepSound = false;

	

	if(InputManagerC::GetInstance()->isResetPortals)
	{
	
		pSoundBank->Play(9, pSoundSourceFoot, XACT_FLAG_SOUNDCUE_AUTORELEASE, NULL);

		
	}

	if(InputManagerC::GetInstance()->isBluePortal || InputManagerC::GetInstance()->isOrangePortal)
	{
		pSoundBank->Play(3, pSoundSourceFoot, XACT_FLAG_SOUNDCUE_AUTORELEASE, NULL);

	}

	if(InputManagerC::GetInstance()->isJump)
	{

		pSoundBank->Play(2, pSoundSourceFoot, XACT_FLAG_SOUNDCUE_AUTORELEASE, NULL); ///***working


		pSoundBank->Stop(12, XACT_FLAG_SOUNDCUE_FLUSH, NULL); //*****
		CreateStepSound = true;

	}

	XACTEngineDoWork();
	
}


void SoundManagerC::shutdown()
{
	// NULL XACT resource pointers
	pStreamingWaveBank = NULL;

	// Free allocated memory
	SAFE_DELETE_ARRAY(pbSoundBank);

	// Release interfaces
	SAFE_RELEASE(pSoundSource);
	SAFE_RELEASE(pSoundBank);
	SAFE_RELEASE(pXACT);
}