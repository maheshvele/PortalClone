#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "gl\glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "SOIL.h"
#include "Box2D.h"
#include <list>
#include "LevelManager.h"
#include "ShapeDraw.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "AnimationManager.h"

const float DEG2RAD = 3.14159f / 180.0f;

void DrawCircle(float_t radius, float_t x, float_t y, uchar8_t r, uchar8_t g, uchar8_t b, bool filled)
{

	glEnable(GL_POINT_SMOOTH);
	if (!filled)
	{
		glDisable(GL_TEXTURE_2D);
		glColor4ub(r, g, b, 0x20);
		// Set the point size
		glPointSize(1.0);
		glBegin(GL_POINTS);
		float_t radiusMax = radius + 2.0f;
		float_t radiusMin = radius - 2.0f;
		for (int i = 0; i < 360; i += 3)
		{
			float degInRad = i*DEG2RAD;
			glVertex2f(x + (cos(degInRad)*radius), y + (sin(degInRad)*radius));
			glVertex2f(x + (cos(degInRad)*radiusMax), y + (sin(degInRad)*radiusMax));
			glVertex2f(x + (cos(degInRad)*radiusMin), y + (sin(degInRad)*radiusMin));
		}
		glEnd();
	}
	else
	{
		glColor4ub(r, g, b, 0xFF);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(radius / 2.0f);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}
}

void DrawLine(float_t startX, float_t startY, float_t endX, float_t endY, uchar8_t r, uchar8_t g, uchar8_t b)
{
	glColor3ub(r, g, b);
	// Draw filtered lines
	glEnable(GL_LINE_SMOOTH);

	glBegin(GL_LINE_STRIP);
	glVertex2f(startX, startY);
	glVertex2f(endX, endY);
	glEnd();

}

void DrawRect(float_t x, float_t y, float_t width, float_t height, uchar8_t r, uchar8_t g, uchar8_t b)
{
	float_t xPositionLeft = x - width / 2;
	float_t yPositionTop = y - height / 2;
	float_t xPositionRight = x + width / 2;
	float_t yPositionBottom = y + height / 2;

	DrawLine(xPositionLeft, yPositionTop, xPositionRight, yPositionTop, r, g, b);
	DrawLine(xPositionRight, yPositionTop, xPositionRight, yPositionBottom, r, g, b);
	DrawLine(xPositionRight, yPositionBottom, xPositionLeft, yPositionBottom, r, g, b);
	DrawLine(xPositionLeft, yPositionBottom, xPositionLeft, yPositionTop, r, g, b);
}

void DrawCursor()
{
	Coord2D *currMousePos;
	currMousePos = InputManagerC::GetInstance()->getCurrentMousePosition();

	DrawCircle(10.0f, currMousePos->x, currMousePos->y, 0xFF, 0xFF, 0xFF, true);
}

void DrawPlayer(GLuint texture)
{
	b2Vec2 playerPosition;
	Sprite currentSprite;
	float x, y;

	playerPosition = PhysicsManagerC::GetInstance()->GetPlayerPosition();
	currentSprite = AnimationManagerC::GetInstance()->CurrentSprite;

	//new***
	x = playerPosition.x * METERS_TO_PIXELS - currentSprite.mWidth / 2;
	y = playerPosition.y * METERS_TO_PIXELS - currentSprite.mHeight / 2;

	DrawCircle(10.0, playerPosition.x * METERS_TO_PIXELS, playerPosition.y * METERS_TO_PIXELS, 0xFF, 0xFF, 0xFF, true);
	DrawRect(playerPosition.x * METERS_TO_PIXELS, playerPosition.y * METERS_TO_PIXELS, PLAYER_WIDHT, PLAYER_HEIGHT, 0xFF, 0xFF, 0xFF);

	if (mouse_x > (playerPosition.x * METERS_TO_PIXELS))
	{
		//new***
		DrawTexturedRect(texture, x, y, currentSprite.mWidth, currentSprite.mHeight,
			currentSprite.u1, currentSprite.u2, currentSprite.v1, currentSprite.v2);

		//get arm info and update!
		DrawPlayerArm(texture, x + 13.0, y + 7.0, 56, 19, 66.0 / 256.0, (66.0 + 56.0) / 256.0, (512.0 - 19.0) / 512.0, (512.0 - 2.0) / 512.0);
	}
	else
	{

		//new inverse***
		DrawTexturedRect(texture, x, y, currentSprite.mWidth, currentSprite.mHeight,
			currentSprite.u2, currentSprite.u1, currentSprite.v1, currentSprite.v2);

		//get arm inverse info and update!
		DrawPlayerArm(texture, x, y, 56, 19, (66.0 + 56.0) / 256.0, 66.0 / 256.0, (512.0 - 19.0) / 512.0, (512.0 - 2.0) / 512.0);
	}
}

void DrawRayCast()
{
	b2Vec2 playerPosition = PhysicsManagerC::GetInstance()->GetPlayerPosition();
	b2Vec2 rayCastPosition;

	rayCastPosition.x = playerPosition.x + (PLAYER_RAYCAST_OFFSET_X - PLAYER_WIDHT / 2) * PIXELS_TO_METERS;
	rayCastPosition.y = playerPosition.y + (PLAYER_RAYCAST_OFFSET_Y - PLAYER_HEIGHT / 2) * PIXELS_TO_METERS;

	if (InputManagerC::GetInstance()->isOrangePortal ||
		InputManagerC::GetInstance()->isBluePortal)
	{
		DrawLine(rayCastPosition.x * METERS_TO_PIXELS, rayCastPosition.y * METERS_TO_PIXELS, mouse_x, mouse_y, 0xFF, 0, 0);
	}
}

void DrawStaticLevelObjects()
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

		DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y,0xFF, 0xFF, 0xFF);
	}

	//Exit triggers
	endPoint1.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.x;
	endPoint1.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.startPoint.y;


	endPoint2.x = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.x;
	endPoint2.y = LevelManagerC::GetInstance()->currentLevelData.exitTrigger.endPoint.y;

	DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0, 0);

	//Non portal walls
	std::list<Line> staticNonPortalObjectList = LevelManagerC::GetInstance()->currentLevelData.nonPortalLines;

	for (std::list<Line>::iterator it = staticNonPortalObjectList.begin(); it != staticNonPortalObjectList.end(); ++it)
	{
		endPoint1.x = (*it).startPoint.x;
		endPoint1.y = (*it).startPoint.y;

		endPoint2.x = (*it).endPoint.x;
		endPoint2.y = (*it).endPoint.y;

		DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0x00, 0x00, 0xFF);
	}

	//Death triggers
	std::list<Line> staticDeathObjectList = LevelManagerC::GetInstance()->currentLevelData.deathTriggers;

	for (std::list<Line>::iterator it = staticDeathObjectList.begin(); it != staticDeathObjectList.end(); ++it)
	{
		endPoint1.x = (*it).startPoint.x;
		endPoint1.y = (*it).startPoint.y;

		endPoint2.x = (*it).endPoint.x;
		endPoint2.y = (*it).endPoint.y;

		DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0x00, 0xFF, 0x00);
	}



	//endPoint1.x = 74.0;
	//endPoint1.y = 720.0;

	//endPoint2.x = 1323.0;
	//endPoint2.y = 720.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 449.0;
	//endPoint1.y = 670.0;

	//endPoint2.x = 864.0;
	//endPoint2.y = 670.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 74.0;
	//endPoint1.y = 547.0;

	//endPoint2.x = 449.0;
	//endPoint2.y = 547.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 1034.0;
	//endPoint1.y = 611.0;

	//endPoint2.x = 1323.0;
	//endPoint2.y = 611.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 74.0;
	//endPoint1.y = 319.0;

	//endPoint2.x = 388.0;
	//endPoint2.y = 319.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 1036.0;
	//endPoint1.y = 453.0;

	//endPoint2.x = 1303.0;
	//endPoint2.y = 453.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 388.0;
	//endPoint1.y = 55.0;

	//endPoint2.x = 1303.0;
	//endPoint2.y = 55.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 74.0;
	//endPoint1.y = 302.0;

	//endPoint2.x = 74.0;
	//endPoint2.y = 720.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 388.0;
	//endPoint1.y = 55.0;

	//endPoint2.x = 388.0;
	//endPoint2.y = 320.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 0xFF, 0xFF); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 449.0;
	//endPoint1.y = 547.0;

	//endPoint2.x = 449.0;
	//endPoint2.y = 670.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 0xFF, 255, 255); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 1036.0;
	//endPoint1.y = 453.0;

	//endPoint2.x = 1036.0;
	//endPoint2.y = 611.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 255, 255, 255); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 1303.0;
	//endPoint1.y = 55.0;

	//endPoint2.x = 1303.0;
	//endPoint2.y = 453.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 255, 255, 255); (endPoint1, endPoint2);
	///******************/

	//endPoint1.x = 864.0;
	//endPoint1.y = 425.0;

	//endPoint2.x = 864.0;
	//endPoint2.y = 670.0;
	//DrawLine(endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, 255, 255, 255); (endPoint1, endPoint2);

}

//Updated***
void DrawPortal(bool isOrangePortal, GLuint texture)
{
	portalStr *portal;
	Sprite PortalSprite;
	float xPos, yPos;

	if (isOrangePortal)
	{
		portal = PhysicsManagerC::GetInstance()->GetOrangePortal();
		PortalSprite = AnimationManagerC::GetInstance()->OrangePortal;
	}
	else
	{
		portal = PhysicsManagerC::GetInstance()->GetBluePortal();
		PortalSprite = AnimationManagerC::GetInstance()->BluePortal;
	}

	if (!portal->isActive)
		return;

	if (portal->normalDirection.y != 0)
	{
		xPos = portal->position.x * METERS_TO_PIXELS - PortalSprite.mHeight / 2;
		yPos = portal->position.y * METERS_TO_PIXELS - PortalSprite.mWidth / 2;

		DrawTexturedRectRotate(texture, xPos, yPos, PortalSprite.mHeight + PORTAL_STRETCH, PortalSprite.mWidth,
			PortalSprite.u1, PortalSprite.u2, PortalSprite.v1, PortalSprite.v2);
	}

	else
	{
		xPos = portal->position.x * METERS_TO_PIXELS - PortalSprite.mWidth / 2;
		yPos = portal->position.y * METERS_TO_PIXELS - PortalSprite.mHeight / 2;

		DrawTexturedRect(texture, xPos, yPos, PortalSprite.mWidth, PortalSprite.mHeight + PORTAL_STRETCH,
			PortalSprite.u1, PortalSprite.u2, PortalSprite.v1, PortalSprite.v2);
	}
}

void DrawTexturedRect(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	//1
	glTexCoord2f(u1, v1);
	glVertex3f(x, y + height, 0.0);

	//2
	glTexCoord2f(u2, v1);
	glVertex3f(x + width, y + height, 0.0);

	//3
	glTexCoord2f(u2, v2);
	glVertex3f(x + width, y, 0.0);

	//4
	glTexCoord2f(u1, v2);
	glVertex3f(x, y, 0.0);


	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//new***
void DrawTexturedRectRotate(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	//1
	glTexCoord2f(u1, v2);
	glVertex3f(x, y + height, 0.0);

	//2
	glTexCoord2f(u1, v1);
	glVertex3f(x + width, y + height, 0.0);

	//3
	glTexCoord2f(u2, v1);
	glVertex3f(x + width, y, 0.0);

	//4
	glTexCoord2f(u2, v2);
	glVertex3f(x, y, 0.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawPlayerArm(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2)
{
	b2Vec2 playerPosition = PhysicsManagerC::GetInstance()->GetPlayerPosition();
	b2Vec2 rayCastPosition;

	rayCastPosition.x = playerPosition.x * METERS_TO_PIXELS + (PLAYER_RAYCAST_OFFSET_X - PLAYER_WIDHT / 2);
	rayCastPosition.y = playerPosition.y *METERS_TO_PIXELS + (PLAYER_RAYCAST_OFFSET_Y - PLAYER_HEIGHT / 2);

	float angleOfRotation = atanf((rayCastPosition.y - mouse_y) / (rayCastPosition.x - mouse_x)) *  RAD_TO_DEGREES;


	glPushMatrix();

	if (playerPosition.x * METERS_TO_PIXELS < mouse_x)
	{
		glTranslatef(x, y, 0);
		glRotatef(angleOfRotation, 0, 0, 1);
		glTranslatef(-x, -y, 0);
	}
	else
	{
		glTranslatef((x + width - 25), (y + height - 20), 0);
		glRotatef(angleOfRotation, 0, 0, 1);
		glTranslatef((-x - width + 15), (-y - height + 20), 0);
	}


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glTexCoord2f(u1, v1);
	glVertex3f(x, y + height, 0.0);

	glTexCoord2f(u2, v1);
	glVertex3f(x + width, y + height, 0.0);

	glTexCoord2f(u2, v2);
	glVertex3f(x + width, y, 0.0);

	glTexCoord2f(u1, v2);
	glVertex3f(x, y, 0.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

float prevX = 0;
float prevY = 0;
void DrawEnemy()
{
	b2Vec2 enemyPosition;

	enemyPosition.x = PhysicsManagerC::GetInstance()->GetEnemy1Position().x;
	enemyPosition.y = PhysicsManagerC::GetInstance()->GetEnemy1Position().y;

	if ((prevX != enemyPosition.x) || (prevY != enemyPosition.y))
	{
		printf("EnemyPosition = %f,%f\n", enemyPosition.x * METERS_TO_PIXELS,
										  enemyPosition.y * METERS_TO_PIXELS);

		prevX = enemyPosition.x;
		prevY = enemyPosition.y;
	}

	DrawRect(enemyPosition.x * METERS_TO_PIXELS, enemyPosition.y * METERS_TO_PIXELS, ENEMY_WIDTH, ENEMY_HEIGHT, 0xFF, 0x00, 0x00);
}

