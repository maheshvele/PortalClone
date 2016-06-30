#define LEVEL_MANAGER_CPP
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
#include <string>
#include <list>
#include "LevelManager.h"


LevelManagerC* LevelManagerC::sInstance = NULL;


LevelManagerC *LevelManagerC::CreateInstance()
{
	if (sInstance != NULL)return sInstance;
	else

		sInstance = new LevelManagerC();
	return sInstance;
}

void LevelManagerC::init()
{
	currentLevel = 1;
	sInstance->currentState = LEVELSTATE_TRANSITION;
}

void LevelManagerC::update()
{
	if (currentState == LEVELSTATE_TRANSITION)
	{
		destroyCurrentLevelData();
		loadLevelData(currentLevel);
		currentState = LEVELSTATE_INIT_LOADLEVEL;
	}
	else if (currentState == LEVELSTATE_INIT_LOADLEVEL)
	{
		currentState = LEVELSTATE_COMPLETE_LOADLEVEL;
	}
}

#define MAX_CHARS_IN_LINE 260

FILE *GetLevelFilePtr(int level)
{
	char fileName[FILENAME_MAX];
	FILE *levelDataPtr;

	switch (level)
	{
	case 1:
		strcpy(fileName, "Levels/level_01.txt");
		break;
	case 2:
		strcpy(fileName, "Levels/level_02.txt");
		break;
	case 3:
		strcpy(fileName, "Levels/level_03.txt");
		break;
	case 4:
		strcpy(fileName, "Levels/level_04.txt");
		break;
	case 5:
		strcpy(fileName, "Levels/level_05.txt");
		break;
	case 6:
		strcpy(fileName, "Levels/level_06.txt");
		break;
	case 7:
		strcpy(fileName, "Levels/level_07.txt");
		break;
	case 8:
		strcpy(fileName, "Levels/level_08.txt");
		break;
	case 9:
		strcpy(fileName, "Levels/level_09.txt");
		break;
	case 10:
		strcpy(fileName, "Levels/level_10.txt");
		break;
	}

	fopen_s(&levelDataPtr, fileName, "r");

	return levelDataPtr;
}

void FillLevelData(char *line, LevelDataStr *currentLevelData)
{
	char *word;
	char *endPtr;
	word = strtok(line, " ");

	

	//Static Lines
	if (strcmp(word, "SL") == 0)
	{
		float data[4];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
		Line staticLine;

		staticLine.startPoint.x = data[0];
		staticLine.startPoint.y = data[1];

		staticLine.endPoint.x = data[2];
		staticLine.endPoint.y = data[3];

		currentLevelData->staticLevelLines.push_back(staticLine);
	}
	//Player SpawnPoint
	else if (strcmp(word, "P") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}

		currentLevelData->playerPosition.x = data[0];
		currentLevelData->playerPosition.y = data[1];

	}
	//Exit Trigger
	else if (strcmp(word, "EX") == 0)
	{
		float data[4];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}

		currentLevelData->exitTrigger.startPoint.x = data[0];
		currentLevelData->exitTrigger.startPoint.y = data[1];
		currentLevelData->exitTrigger.endPoint.x = data[2];
		currentLevelData->exitTrigger.endPoint.y = data[3];
	}
	//Dynamic Lines
	else if (strcmp(word, "DL") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			data[i++] = strtof(word, &endPtr);
		}
	}
	//Static Boxes
	else if (strcmp(word, "SB") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
	}
	//Dynamic Boxes
	else if (strcmp(word, "DB") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
	}
	//Enemies
	else if (strcmp(word, "EN1") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
		currentLevelData->enemy1.x = data[0];
		currentLevelData->enemy1.y = data[1];
		currentLevelData->isEnemy1Active = true;
	}
	else if (strcmp(word, "EN2") == 0)
	{
		float data[2];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
		currentLevelData->enemy2.x = data[0];
		currentLevelData->enemy2.y = data[1];
		currentLevelData->isEnemy2Active = true;
	}
	else if (strcmp(word, "DT") == 0)
	{
		float data[4];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
		Line staticLine;

		staticLine.startPoint.x = data[0];
		staticLine.startPoint.y = data[1];

		staticLine.endPoint.x = data[2];
		staticLine.endPoint.y = data[3];

		currentLevelData->deathTriggers.push_back(staticLine);
	}

	else if (strcmp(word, "NP") == 0)
	{
		float data[4];
		int i = 0;
		while (word != NULL)
		{
			word = strtok(NULL, " ");
			if (word)
				data[i++] = strtof(word, &endPtr);
		}
		Line staticLine;

		staticLine.startPoint.x = data[0];
		staticLine.startPoint.y = data[1];

		staticLine.endPoint.x = data[2];
		staticLine.endPoint.y = data[3];

		currentLevelData->nonPortalLines.push_back(staticLine);

	}
}

void LevelManagerC::loadLevelData(int level)
{
	FILE *currentLevelPtr;
	char line[MAX_CHARS_IN_LINE];
	int numLines = 0;

	currentLevelPtr = GetLevelFilePtr(level);

	if (!currentLevelPtr)
	{
		printf("Major Error: Unable to open level_%d file", level);
		return;
	}

	sInstance->currentLevelData.isEnemy1Active = false;
	sInstance->currentLevelData.isEnemy2Active = false;

	while (!feof(currentLevelPtr))
	{
		fgets(line, MAX_CHARS_IN_LINE, currentLevelPtr);
		
		if (numLines == 0)
		{
			char *fileName;
			fileName = strtok(line, "\n");
			strcpy(currentLevelData.levelImage, fileName);
		}
		else
			FillLevelData(line, &sInstance->currentLevelData);
		numLines++;
	}

	fclose(currentLevelPtr);
}

void LevelManagerC::destroyCurrentLevelData()
{
	std::list<Line>::iterator iter;

	//verifies if any of the lists have data
	if (sInstance->currentLevelData.staticLevelLines.size() == 0 && sInstance->currentLevelData.deathTriggers.size() == 0
		&& sInstance->currentLevelData.nonPortalLines.size() == 0)
	{
		return;
	}
		
	
	//clears static lines list
	iter = sInstance->currentLevelData.staticLevelLines.begin();

	while (iter != sInstance->currentLevelData.staticLevelLines.end())
	{
		iter = currentLevelData.staticLevelLines.erase(iter);
	}

	//clears death triggers list
	iter = sInstance->currentLevelData.deathTriggers.begin();

	while (iter != sInstance->currentLevelData.deathTriggers.end())
	{
		iter = currentLevelData.deathTriggers.erase(iter);
	}

	//clears the non portal walls list
	iter = sInstance->currentLevelData.nonPortalLines.begin();

	while (iter != sInstance->currentLevelData.nonPortalLines.end())
	{
		iter = currentLevelData.nonPortalLines.erase(iter);
	}

}
