#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define CONTROL_LEFTTHUMBSTICK  0
#define CONTROL_RIGHTTHUMBSTICK 1
#define CONTROL_BODY            2
#define CONTROL_BACKBUTTON      3
#define CONTROL_STARTBUTTON     4
#define CONTROL_ABUTTON         5
#define CONTROL_BBUTTON         6
#define CONTROL_XBUTTON         7
#define CONTROL_YBUTTON         8
#define CONTROL_WHITEBUTTON     9
#define CONTROL_BLACKBUTTON    10
#define CONTROL_CORDSTUB       11
#define CONTROL_GASKETS        12
#define CONTROL_MEMCARDSLOT    13
#define CONTROL_LEFTTRIGGER    14
#define CONTROL_RIGHTTRIGGER   15
#define CONTROL_DPAD           16
#define CONTROL_JEWEL          17
#define NUM_CONTROLS           18

class InputManagerC
{
public:
	static InputManagerC	*CreateInstance();
	static InputManagerC	*GetInstance() { return sInstance; };
	void					init();
	void					update(XBGAMEPAD *m_pGamePad);
	void					shutdown();
	//checkout gameframework.cpp
	Coord2D*				getCurrentMousePosition(){ return &sInstance->mCurrentMousePosition;  };
	Coord2D*				getCurrentCrossHairPosition(){return &sInstance->mCurrentCrossHairPosition; };

	bool					isMoveRight;
	bool					isMoveLeft;
	bool					isJump;
	bool					isOrangePortal;
	bool					isBluePortal;
	bool					isResetPortals;
	bool					isHoldCube;
	
	//keyboard interface?

private:
	InputManagerC(){};
	Coord2D					mCurrentMousePosition;
	Coord2D					mCurrentCrossHairPosition;
	bool					mActiveControl[NUM_CONTROLS];
	static InputManagerC *sInstance;
};

void GetWorldCoordinates(int mouse_x, int mouse_y, float *gl_mouse_x, float *gl_mouse_y);

#endif