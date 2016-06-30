#define RENDERING_MANAGER_CPP
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include "baseTypes.h"
#include  "game.h"
#include "gamedefs.h"
#include "Box2D.h"
#include <list>
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "AnimationManager.h"
#include "LevelManager.h"
#include "Resource.h"

#define PI_2 1.57079632679489661923f

extern LPDIRECT3DDEVICE8 g_pd3dDevice;


float prevX = 0;
float prevY = 0;

RenderingManagerC* RenderingManagerC::sInstance = NULL;


//-----------------------------------------------------------------------------
// Name: LoadPackedResource()
// Desc: Loads all the texture resources from the given XPR.
//-----------------------------------------------------------------------------
HRESULT RenderingManagerC::LoadPackedResource()
{
	// Open the file to read the XPR headers
	FILE* file = fopen( "D:\\Media\\Resource.xpr", "rb" );
	if( NULL == file )
		return E_FAIL;

	// Read in and verify the XPR magic header
	XPR_HEADER xprh;
	if( fread( &xprh, sizeof(XPR_HEADER), 1, file ) != 1 )
	{
		fclose(file);
		return E_FAIL;
	}

	if( xprh.dwMagic != XPR_MAGIC_VALUE )
	{
		OutputDebugStringA( "ERROR: Invalid Xbox Packed Resource (.xpr) file" );
		fclose( file );
		return E_INVALIDARG;
	}

	// Compute memory requirements
	DWORD dwSysMemDataSize = xprh.dwHeaderSize - sizeof(XPR_HEADER);
	DWORD dwVidMemDataSize = xprh.dwTotalSize - xprh.dwHeaderSize;

	// Allocate memory
	g_pResourceSysMemData = new BYTE[dwSysMemDataSize];
	g_pResourceVidMemData = (BYTE*)D3D_AllocContiguousMemory( dwVidMemDataSize, D3DTEXTURE_ALIGNMENT );

	// Read in the data from the file
	if( fread( g_pResourceSysMemData, dwSysMemDataSize, 1, file ) != 1 ||
		fread( g_pResourceVidMemData, dwVidMemDataSize, 1, file ) != 1 )

	{
		delete[] g_pResourceSysMemData;
		D3D_FreeContiguousMemory( g_pResourceVidMemData );
		fclose( file );
		return E_FAIL;
	}

	// Done with the file
	fclose( file );

	// Loop over resources, calling Register()
	BYTE* pData = g_pResourceSysMemData;

	for( DWORD i = 0; i < resource_NUM_RESOURCES; i++ )
	{
		// Get the resource
		LPDIRECT3DRESOURCE8 pResource = (LPDIRECT3DRESOURCE8)pData;

		// Register the resource
		pResource->Register( g_pResourceVidMemData );

		// Advance the pointer
		switch( pResource->GetType() )
		{
		case D3DRTYPE_TEXTURE:       pData += sizeof(D3DTexture);       break;
		case D3DRTYPE_VOLUMETEXTURE: pData += sizeof(D3DVolumeTexture); break;
		case D3DRTYPE_CUBETEXTURE:   pData += sizeof(D3DCubeTexture);   break;
		case D3DRTYPE_VERTEXBUFFER:  pData += sizeof(D3DVertexBuffer);  break;
		case D3DRTYPE_INDEXBUFFER:   pData += sizeof(D3DIndexBuffer);   break;
		case D3DRTYPE_PALETTE:       pData += sizeof(D3DPalette);       break;
		default:                     return E_FAIL;
		}
	}


	return S_OK;
}



RenderingManagerC *RenderingManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new RenderingManagerC();
	return sInstance;
}
void RenderingManagerC::init(D3DPRESENT_PARAMETERS m_d3dpp)
{
	g_pD3D       = NULL; // Used to create the D3DDevice
	g_pResourceSysMemData = NULL; // Sysmem data for the packed resource
	g_pResourceVidMemData = NULL; // Vidmem data for the packed resource
	g_pVB				  = NULL;          // Buffer to hold vertices
	g_backGroundTexture   = NULL; // Our texture
	g_playerTexture       = NULL;
	g_crossHairTexture    = NULL;


	m_d3dpp.BackBufferWidth = 1280;
	m_d3dpp.BackBufferHeight = 720;
	m_d3dpp.Flags = 80;

	g_pd3dDevice->Reset( &m_d3dpp );


	D3DXCreateSprite(g_pd3dDevice, &backGround);
	D3DXCreateSprite(g_pd3dDevice, &player);
	D3DXCreateSprite(g_pd3dDevice, &crossHair);
	D3DXCreateSprite(g_pd3dDevice, &playerSpriteSheet);
	D3DXCreateSprite(g_pd3dDevice, &playerHand);
	D3DXCreateSprite(g_pd3dDevice, &turret);

	

	LoadPackedResource();
	// Get access to the texture
	g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level1_OFFSET ];
	g_playerTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_player_OFFSET];
	g_crossHairTexture= (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_crossHair_OFFSET];
	g_playerSpriteSheetTexture = (LPDIRECT3DTEXTURE8) &g_pResourceSysMemData[ resource_spriteSheet_OFFSET];
	g_turretTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_turret_OFFSET];
	
	
	PlayPortalSound = false;

}

void RenderingManagerC::DrawTurrents()
{
	D3DXVECTOR2 crossHairScale = D3DXVECTOR2();
	//playerPos.y -= 16.0f;

	D3DXVECTOR2 turretPos;;

	turretPos.x = PhysicsManagerC::GetInstance()->GetEnemy1Position().x * METERS_TO_PIXELS - 50.0f;
	turretPos.y = PhysicsManagerC::GetInstance()->GetEnemy1Position().y * METERS_TO_PIXELS - 50.0f;
	
	//DrawBox(turretPos.x, turretPos.y, turretPos.x + ENEMY_WIDTH, turretPos.y + ENEMY_HEIGHT, 0x00000000, 0xFFFFFFFF);
	
	sInstance->turret->Draw(g_turretTexture, NULL, NULL, NULL, 0.0f, &turretPos, 0xFFFFFFFF);
}

void RenderingManagerC::DrawPlayer()
{
	//D3DXVECTOR2 playerScale = D3DXVECTOR2(47.0f/64.0f, 77.0f/128.0f);
	D3DXVECTOR2 playerScale = D3DXVECTOR2(1.0f, 1.0f);
	D3DXVECTOR2 flippedScale = D3DXVECTOR2(-1.0f, 1.0f);
	D3DXVECTOR2 playerPos ;
	D3DXVECTOR2 playerBox;
	D3DXVECTOR2 playerHandPos;


	playerPos.x = PhysicsManagerC::GetInstance()->GetPlayerPosition().x * METERS_TO_PIXELS ;
	playerPos.y = PhysicsManagerC::GetInstance()->GetPlayerPosition().y * METERS_TO_PIXELS ;

	playerBox.x = PhysicsManagerC::GetInstance()->GetPlayerPosition().x * METERS_TO_PIXELS ;
	playerBox.y = PhysicsManagerC::GetInstance()->GetPlayerPosition().y * METERS_TO_PIXELS ;

	playerBox.x -= PLAYER_WIDHT/2;
	playerBox.y -= PLAYER_HEIGHT/2;

	//DrawBox(playerBox.x, playerBox.y, playerBox.x + PLAYER_WIDHT, playerBox.y + PLAYER_HEIGHT, 0x00000000, 0xFFFFFFFF);

	playerPos.x -= PLAYER_WIDHT/2;
	playerPos.y -= PLAYER_HEIGHT/2;

	if( (prevX != playerPos.x) || (prevY != playerPos.y))
	{
		char debugStr[200];
		sprintf(debugStr, "Player Position : %f,%f\n", playerPos.x, playerPos.y);
		OUTPUT_DEBUG_STRING(debugStr);
		prevX = playerPos.x;
		prevY = playerPos.y;
	}
	
	b2Vec2 playerPosition = PhysicsManagerC::GetInstance()->GetPlayerPosition();
	b2Vec2 rayCastPosition;

	rayCastPosition.x = playerPosition.x * METERS_TO_PIXELS + (PLAYER_RAYCAST_OFFSET_X - PLAYER_WIDHT / 2) ;
	rayCastPosition.y = playerPosition.y * METERS_TO_PIXELS + (PLAYER_RAYCAST_OFFSET_Y - PLAYER_HEIGHT / 2) ;

	playerHandPos.x = playerPos.x + 15.0f;
	playerHandPos.y = playerPos.y + 10.;

	float angleOfRotation = atanf((rayCastPosition.y - InputManagerC::GetInstance()->getCurrentCrossHairPosition()->y) / (rayCastPosition.x - InputManagerC::GetInstance()->getCurrentCrossHairPosition()->x)) ;
	
	if(InputManagerC::GetInstance()->getCurrentCrossHairPosition()->x > playerPos.x)
	{
		player->Draw(g_playerSpriteSheetTexture, &AnimationManagerC::GetInstance()->CurrentSpriteRect , &playerScale, NULL, 0.0f, &playerPos, 0xFFFFFFFF);
		playerHand->Draw(g_playerSpriteSheetTexture, &AnimationManagerC::GetInstance()->PlayerHand, &playerScale, NULL, -angleOfRotation, &playerHandPos, 0xFFFFFFFF);
	}
	else
	{
		playerPos.x = playerPos.x + (AnimationManagerC::GetInstance()->CurrentSpriteRect.right - AnimationManagerC::GetInstance()->CurrentSpriteRect.left);
		playerHandPos.x = playerHandPos.x + 10.0f;
		player->Draw(g_playerSpriteSheetTexture, &AnimationManagerC::GetInstance()->CurrentSpriteRect , &flippedScale, NULL, 0.0f, &playerPos, 0xFFFFFFFF);
		playerHand->Draw(g_playerSpriteSheetTexture, &AnimationManagerC::GetInstance()->PlayerHand, &flippedScale, NULL, -angleOfRotation, &playerHandPos, 0xFFFFFFFF);
	}
}

void RenderingManagerC::DrawCrossHair()
{
	D3DXVECTOR2 crossHairScale = D3DXVECTOR2();
	//playerPos.y -= 16.0f;

	D3DXVECTOR2 crossHairPos;

	crossHairPos.x = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->x;
	crossHairPos.y = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->y;

	crossHairPos.x -= 16.0f;
	crossHairPos.y -= 16.0f;

	sInstance->crossHair->Draw(g_crossHairTexture, NULL, NULL, NULL, 0.0f, &crossHairPos, 0xFFFFFFFF);
}

void RenderingManagerC::DrawRaycast()
{
	Coord2D crossHair;
	Coord2D playerPos;

	crossHair.x = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->x;
	crossHair.y = InputManagerC::GetInstance()->getCurrentCrossHairPosition()->y;
	
	playerPos.x = PhysicsManagerC::GetInstance()->GetPlayerPosition().x * METERS_TO_PIXELS;
	playerPos.y = PhysicsManagerC::GetInstance()->GetPlayerPosition().y * METERS_TO_PIXELS - 10.0f;

	if(InputManagerC::GetInstance()->isBluePortal || 
		InputManagerC::GetInstance()->isOrangePortal)
	{
		//RenderingManagerC::DrawBox(playerPos.x, playerPos.y, crossHair.x, crossHair.y, 0x00000000, 0xFFFF0000);
	}
}
void RenderingManagerC::DrawPortal(bool isOrangePortal)
{
			portalStr *portal;
			Sprite PortalSprite;
			float xPos, yPos;
			D3DXVECTOR2 Position;
			D3DXVECTOR2 PortalScale; 
			D3DXVECTOR2 RotationCenter;

			//PortalSprite.mHeight = PORTAL_HEIGHT;
			//PortalSprite.mWidth = PORTAL_WIDTH;

			PortalSprite.mHeight = 29.0f;
			PortalSprite.mWidth = 59.0f;

			PortalScale.x = 1.0f;
			PortalScale.y = 1.5f;
		
			if (isOrangePortal)
			{
				portal = PhysicsManagerC::GetInstance()->GetOrangePortal();
				CurrentPortal = AnimationManagerC::GetInstance()->OrangeRect;
			}
			else
			{
				portal = PhysicsManagerC::GetInstance()->GetBluePortal();
				CurrentPortal = AnimationManagerC::GetInstance()->BlueRect;
			}
		
			if (!portal->isActive)
				return;
		
			if (portal->normalDirection.y != 0)
			{
				Position.x = portal->position.x * METERS_TO_PIXELS - PortalSprite.mHeight / 2;
				Position.y = portal->position.y * METERS_TO_PIXELS - PortalSprite.mWidth / 2;

				RotationCenter.x = PortalSprite.mHeight / 2;
				RotationCenter.y = PortalSprite.mWidth / 2;
		
				//PlayPortalSound = true;
				player->Draw(g_playerSpriteSheetTexture, &CurrentPortal , &PortalScale, &RotationCenter, PI_2, &Position, 0xFFFFFFFF);
			}
		
			else
			{

				Position.x = portal->position.x * METERS_TO_PIXELS - PortalSprite.mHeight / 2;
				Position.y = portal->position.y * METERS_TO_PIXELS - PortalSprite.mWidth / 2;

				//PlayPortalSound = true;
				player->Draw(g_playerSpriteSheetTexture, &CurrentPortal , &PortalScale, NULL, 0.0f, &Position, 0xFFFFFFFF);

			}
}

void RenderingManagerC::DrawBackground()
{
	D3DXVECTOR2 scale = D3DXVECTOR2(1280.0f/2048.0f, 720.0f/1024.0f);
	D3DXVECTOR2 topLeft = D3DXVECTOR2(10.0, 0.0);

	switch (LevelManagerC::GetInstance()->currentLevel)
	{
	case 1:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level1_OFFSET ];
		break;
	case 2:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level2_OFFSET ];
		break;
	case 3:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level3_OFFSET ];
		break;
	case 4:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level4_OFFSET ];
		break;
	case 5:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level5_OFFSET ];
		break;
	case 6:
		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level6_OFFSET ];
		break;
	case 7:
//		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level7_OFFSET ];
		break;
	case 8:
//		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level8_OFFSET ];
		break;
	case 9:
//		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level9_OFFSET ];
		break;
	case 10:
//		g_backGroundTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[ resource_level10_OFFSET ];
		break;
	}

	backGround->Draw(g_backGroundTexture,NULL, &scale, NULL, 0.0f, &topLeft, 0xFFFFFFFF);
}

void RenderingManagerC::DrawBox( FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,DWORD dwFillColor, DWORD dwOutlineColor )
{
	D3DXVECTOR4 v[4];
	v[0] = D3DXVECTOR4( x1-0.5f, y1-0.5f, 0, 0 );
	v[1] = D3DXVECTOR4( x2-0.5f, y1-0.5f, 0, 0 );
	v[2] = D3DXVECTOR4( x2-0.5f, y2-0.5f, 0, 0 );
	v[3] = D3DXVECTOR4( x1-0.5f, y2-0.5f, 0, 0 );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW );

	// Render the box
	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwFillColor );
	g_pd3dDevice->DrawVerticesUP( D3DPT_QUADLIST, 4, v, sizeof(v[0]) );

	// Render the lines
	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwOutlineColor );
	g_pd3dDevice->DrawVerticesUP( D3DPT_LINELOOP, 4, v, sizeof(v[0]) );
}

void RenderingManagerC::DrawStaticObjects()
{

//Level Borders
Coord2D endPoint1, endPoint2;

std::list<Line> staticObjectList = LevelManagerC::GetInstance()->currentLevelData.staticLevelLines;

for (std::list<Line>::iterator it = staticObjectList.begin(); it != staticObjectList.end(); ++it)
{
	endPoint1.x = (*it).startPoint.x;
	endPoint1.y = (*it).startPoint.y;

	endPoint2.x = (*it).endPoint.x;
	endPoint2.y = (*it).endPoint.y;

	//RenderingManagerC::DrawBox(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0x00000000, 0xFFFFFFFF);

	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y,0xFF, 0xFF, 0xFF);
}

//Exit triggers
endPoint1.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.x;
endPoint1.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.y;


endPoint2.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.x;
endPoint2.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.y;

//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0, 0);

//Non portal walls
std::list<Line> staticNonPortalObjectList = LevelManagerC::GetInstance()->currentLevelData.nonPortalLines;

for (std::list<Line>::iterator it = staticNonPortalObjectList.begin(); it != staticNonPortalObjectList.end(); ++it)
{
	endPoint1.x = (*it).startPoint.x;
	endPoint1.y = (*it).startPoint.y;

	endPoint2.x = (*it).endPoint.x;
	endPoint2.y = (*it).endPoint.y;

	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0x00, 0x00, 0xFF);
}

//Death triggers
std::list<Line> staticDeathObjectList = LevelManagerC::GetInstance()->currentLevelData.deathTriggers;

for (std::list<Line>::iterator it = staticDeathObjectList.begin(); it != staticDeathObjectList.end(); ++it)
{
	endPoint1.x = (*it).startPoint.x;
	endPoint1.y = (*it).startPoint.y;

	endPoint2.x = (*it).endPoint.x;
	endPoint2.y = (*it).endPoint.y;

	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0x00, 0xFF, 0x00);
}

}
void RenderingManagerC::update()
{

	

	// Clear the backbuffer to a blue color
	    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 
	                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0L );
	
	    // Begin the scene
	    g_pd3dDevice->BeginScene();
	
		DrawBackground();
		DrawPlayer();
		
		DrawRaycast();

		DrawPortal(true);
		DrawPortal(false);

		DrawCrossHair();
		//DrawBox(71.0f, 673.0f, 1218.0f, 673.0f, 0x00000000, 0xFFFFFFFF);
		DrawStaticObjects();

	

		DrawTurrents();
	    // End the scene
	    g_pd3dDevice->EndScene();

		// Present the backbuffer contents to the display
		g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void RenderingManagerC::shutdown()
{

}
