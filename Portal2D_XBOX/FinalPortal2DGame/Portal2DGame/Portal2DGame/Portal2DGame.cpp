// Portal2DGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <d3dx8core.h>
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include "baseTypes.h"
#include "Box2D.h"
#include <list>
#include "LevelManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "AnimationManager.h"
#include "AudioManager.h"
#include "RenderingManager.h"
#include <xact.h>
#include <dsstdfx.h>
#include "SoundManager.h"
#include "Resource.h"

//-----------------------------------------------------------------------------
// Name: class CXBoxSample
// Desc: Application class. The base class provides just about all the
//       functionality we want, so we're just supplying stubs to interface with
//       the non-C++ functions of the app.
//-----------------------------------------------------------------------------
class Portal2DGameC : public CXBApplication
{
	// Valid app states
	enum APPSTATE 
	{ 
		APPSTATE_STARTGAME_SCREEN=0, 
		APPSTATE_LEVELSELECT_SCREEN,
		APPSTATE_INGAME_SCREEN,
		APPSTATE_LOADGAME_SCREEN,
		APPSTATE_ENDGAME_SCREEN,
		APPSTATE_MAX,
	};

	// General application members
	APPSTATE           m_AppState;         // State of the app
	CXBPackedResource  m_xprResource;      // Packed resources for the app
	CXBFont            m_Font16;           // 16-point font class
	CXBFont            m_Font12;           // 12-point font class

	// Active gamepad
	XBGAMEPAD*         m_pGamepad;
	DWORD            m_dwVideoFlags;
	DWORD            m_dwCurrentMode;

protected:
	HRESULT Initialize();
	HRESULT FrameMove();
	HRESULT Render();
	

public:
	Portal2DGameC();
	HRESULT ShutDown();
};



//-----------------------------------------------------------------------------
// Name: main()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
void __cdecl main()
{
	Portal2DGameC gameApp;

	if(FAILED(gameApp.Create()))
		return;

	gameApp.Run();

	gameApp.ShutDown();
}

Portal2DGameC::Portal2DGameC() : CXBApplication()
{
	m_AppState              = APPSTATE_STARTGAME_SCREEN;
	m_pGamepad              = &m_DefaultGamepad;
	m_dwVideoFlags			= XGetVideoFlags();
}

HRESULT Portal2DGameC::Initialize()
{
	 OUTPUT_DEBUG_STRING( "Portal2D: Inside the Initialization function\n" );

	InputManagerC::CreateInstance();
	PhysicsManagerC::CreateInstance();
	RenderingManagerC::CreateInstance();
	AnimationManagerC::CreateInstance();
	SoundManagerC::CreateInstance();
	LevelManagerC::CreateInstance();

	LevelManagerC::GetInstance()->init();
	InputManagerC::GetInstance()->init();
	PhysicsManagerC::GetInstance()->init();
	RenderingManagerC::GetInstance()->init(m_d3dpp);
	AnimationManagerC::GetInstance()->init();
	SoundManagerC::GetInstance()->init();
	
	return S_OK;
}

HRESULT Portal2DGameC::FrameMove()
{
	LevelManagerC::GetInstance()->update();
	InputManagerC::GetInstance()->update(m_pGamepad);
	PhysicsManagerC::GetInstance()->update();
	AnimationManagerC::GetInstance()->update(m_fFPS);
	//SoundManagerC::GetInstance()->update();

	return S_OK;
}


HRESULT Portal2DGameC::Render()
{
	RenderingManagerC::GetInstance()->update();
	SoundManagerC::GetInstance()->update();
	return S_OK;
}

HRESULT Portal2DGameC::ShutDown()
{
	PhysicsManagerC::GetInstance()->shutdown();
	InputManagerC::GetInstance()->shutdown();
	RenderingManagerC::GetInstance()->shutdown();

	delete LevelManagerC::GetInstance();
	delete PhysicsManagerC::GetInstance();
	delete InputManagerC::GetInstance();
	delete RenderingManagerC::GetInstance();
	delete AnimationManagerC::GetInstance();
	delete SoundManagerC::GetInstance();

	return S_OK;
}

