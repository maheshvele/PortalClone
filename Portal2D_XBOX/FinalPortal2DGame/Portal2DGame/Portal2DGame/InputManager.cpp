#include <stdio.h>
#include <xbapp.h>
#include <xbfont.h>
#include <xbmesh.h>
#include <xbutil.h>
#include <xbresource.h>
#include <xgraphics.h>
#include "baseTypes.h"														// Header File For The NeHeGL Basecode
#include "InputManager.h"

InputManagerC* InputManagerC::sInstance = NULL;

InputManagerC *InputManagerC::CreateInstance()
{
	if(sInstance!=NULL)return sInstance;
	else

	sInstance = new InputManagerC();
	return sInstance;
}

void InputManagerC::init()
{
	sInstance->isMoveRight = false;
	sInstance->isMoveLeft= false;
	sInstance->isJump = false;
	sInstance->isOrangePortal = false;
	sInstance->isBluePortal = false;
	sInstance->isHoldCube = false;
	sInstance->isResetPortals = false;

	for(int i = 0; i < NUM_CONTROLS; i++)
	{
		mActiveControl[i] = FALSE;
	}
}

float prevCrossHairPositionX = 0.0f;
float prevCrossHairPositionY = 0.0f;

//handles keyboard processing
void InputManagerC::update(XBGAMEPAD *m_pGamepad)
{
	
	mActiveControl[CONTROL_LEFTTHUMBSTICK]  = ( m_pGamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB ||
											    m_pGamepad->fX1 || m_pGamepad->fY1 ) ? TRUE : FALSE;
	mActiveControl[CONTROL_RIGHTTHUMBSTICK] = ( m_pGamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ||
												m_pGamepad->fX2 || m_pGamepad->fY2 ) ? TRUE : FALSE;
	mActiveControl[CONTROL_BACKBUTTON]  = ( m_pGamepad->wButtons & XINPUT_GAMEPAD_BACK ) ? TRUE : FALSE;
	mActiveControl[CONTROL_STARTBUTTON] = ( m_pGamepad->wButtons & XINPUT_GAMEPAD_START ) ? TRUE : FALSE;
	mActiveControl[CONTROL_DPAD]        = ( m_pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP || 
		m_pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN || 
		m_pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT || 
		m_pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) ? TRUE : FALSE;
	mActiveControl[CONTROL_ABUTTON]      = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_A];
	mActiveControl[CONTROL_BBUTTON]      = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_B];
	mActiveControl[CONTROL_XBUTTON]      = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_X];
	mActiveControl[CONTROL_YBUTTON]      = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_Y];
	mActiveControl[CONTROL_BLACKBUTTON]  = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_BLACK];
	mActiveControl[CONTROL_WHITEBUTTON]  = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_WHITE];
	mActiveControl[CONTROL_LEFTTRIGGER]  = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER];
	mActiveControl[CONTROL_RIGHTTRIGGER] = m_pGamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER];
	
	if(mActiveControl[CONTROL_LEFTTHUMBSTICK])
	{
		char debugString[200];
		sprintf( debugString, "LeftStick.x = %f\n", m_pGamepad->fX1);
		OUTPUT_DEBUG_STRING(debugString);
		//OUTPUT_DEBUG_STRING(debugString);

		if(m_pGamepad->fX1 == 1.0f)
		{
			isMoveRight = true;
			//OUTPUT_DEBUG_STRING(debugString);
			//OUTPUT_DEBUG_STRING("moveRight\n");
		}
		else if(m_pGamepad->fX1 == -1.0f)
		{
			isMoveLeft = true;
			//OUTPUT_DEBUG_STRING("moveLeft\n");
			
		}
		else
		{
			isMoveLeft = false;
			isMoveRight = false;
	
		}
	}

	if(mActiveControl[CONTROL_RIGHTTHUMBSTICK])
	{
		char debugString[200];
	//	sprintf( debugString, "RightStick.x, RightStick.y = %d,%d\n", m_pGamepad->sThumbRX, m_pGamepad->sThumbRY );
	//	OUTPUT_DEBUG_STRING(debugString);

		FLOAT fThumbRX = 640.0f + 700.0f*(m_pGamepad->sThumbRX)/32767.5f;
		FLOAT fThumbRY = 360.0f - 400.0f*(m_pGamepad->sThumbRY)/32767.5f;

		mCurrentCrossHairPosition.x = fThumbRX;
		mCurrentCrossHairPosition.y = fThumbRY;

		//Convert the analog stick into screen coordinates
		if( (prevCrossHairPositionX != fThumbRX) || (prevCrossHairPositionY!=fThumbRY))
		{
			sprintf(debugString, "screenPosition = (%f, %f)\n", fThumbRX, fThumbRY);
			OUTPUT_DEBUG_STRING(debugString);
		}

		prevCrossHairPositionX = fThumbRX;
		prevCrossHairPositionY = fThumbRY;
	}
	
	if(mActiveControl[CONTROL_LEFTTRIGGER])
	{
		OUTPUT_DEBUG_STRING("Left Trigger Pressed\n");
		isOrangePortal = true;
	}
	else
	{
		isOrangePortal = false;
	}

	if(mActiveControl[CONTROL_RIGHTTRIGGER])
	{
		//OUTPUT_DEBUG_STRING("Right Trigger Pressed\n");
		isBluePortal = true;
	}
	else
	{
		isBluePortal = false;
	}

	if(mActiveControl[CONTROL_ABUTTON])
	{
		//OUTPUT_DEBUG_STRING("A button pressed\n");
		isJump = true;
	}
	else
	{
		isJump = false;
	}

	if(mActiveControl[CONTROL_BBUTTON])
	{
		isHoldCube = true;
	}
	else
	{
		isHoldCube = false;
	}

	if(mActiveControl[CONTROL_XBUTTON])
	{
		isResetPortals = true;
	}
	else
	{
		isResetPortals = false;
	}
}

void InputManagerC::shutdown()
{
	return;
}
//
//void GetWorldCoordinates(int mouse_x, int mouse_y, float *gl_mouse_x, float *gl_mouse_y)
//{
//	/*GLint viewport[4];
//	GLdouble modelview[16];
//	GLdouble projection[16];
//	GLfloat winX, winY, winZ;
//	GLdouble posX, posY, posZ;
//	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//	glGetIntegerv(GL_VIEWPORT, viewport);
//	winX = (float)mouse_x;
//	winY = (float)viewport[3] - (float)mouse_y;
//	glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
//	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
//
//	*gl_mouse_x = (float)posX;
//	*gl_mouse_y = (float)posY;*/
//}

