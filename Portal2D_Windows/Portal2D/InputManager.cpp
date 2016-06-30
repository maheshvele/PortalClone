#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include "gl\glut.h"
#include "baseTypes.h"
#include "openglframework.h"														// Header File For The NeHeGL Basecode
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
}

//handles keyboard processing
void InputManagerC::update(void)
{
	if (g_keys->keyDown[VK_ESCAPE])								// Is ESC Being Pressed?
	{
		TerminateApplication(g_window);						// Terminate The Program
	}

	if (g_keys->keyDown[VK_F1])									// Is F1 Being Pressed?
	{
		ToggleFullscreen(g_window);							// Toggle Fullscreen Mode
	}

	if (g_keys->keyDown[VK_RIGHT] || 
		g_keys->keyDown['D'])
	{
		sInstance->isMoveRight = true;
	}
	else
	{
		sInstance->isMoveRight = false;
	}

	if (g_keys->keyDown[VK_LEFT] ||
		g_keys->keyDown['A'])
	{
		sInstance->isMoveLeft = true;
	}
	else
	{
		sInstance->isMoveLeft = false;
	}
	
	if (g_keys->keyDown[VK_SPACE])
	{
		sInstance->isJump = true;
	}
	else
	{
		sInstance->isJump = false;
	}

	if (g_keys->keyDown[VK_RBUTTON])
	{
		sInstance->isBluePortal = true;
	}
	else
	{
		sInstance->isBluePortal = false;
	}

	if (g_keys->keyDown[VK_LBUTTON])
	{
		sInstance->isOrangePortal = true;
	}
	else
	{
		sInstance->isOrangePortal = false;
	}

	if (g_keys->keyDown['R'])
	{
		sInstance->isResetPortals = true;
	}
	else
	{
		sInstance->isResetPortals = false;
	}

	sInstance->mCurrentMousePosition.x = mouse_x;
	sInstance->mCurrentMousePosition.y = mouse_y;

//	printf("mouse X, mouse Y = (%d, %d)\n", mouse_x, mouse_y);
	GetWorldCoordinates(mouse_x, mouse_y, &gl_mouse_x, &gl_mouse_y);

//	printf("GL_mouse X, GL_mouse Y = (%f, %f)\n", gl_mouse_x, gl_mouse_y);
}

void InputManagerC::shutdown()
{

}

void GetWorldCoordinates(int mouse_x, int mouse_y, float *gl_mouse_x, float *gl_mouse_y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (float)mouse_x;
	winY = (float)viewport[3] - (float)mouse_y;
	glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	*gl_mouse_x = (float)posX;
	*gl_mouse_y = (float)posY;
}

