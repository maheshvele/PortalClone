#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
class InputManagerC
{
public:
	static InputManagerC	*CreateInstance();
	static InputManagerC	*GetInstance() { return sInstance; };
	void					init();
	void					update();
	void					shutdown();
	//checkout gameframework.cpp
	Coord2D*					getCurrentMousePosition(){ return &sInstance->mCurrentMousePosition;  };

	bool					isMoveRight;
	bool					isMoveLeft;
	bool					isJump;
	bool					isOrangePortal;
	bool					isBluePortal;
	bool					isResetPortals;
	//keyboard interface?

private:
	InputManagerC(){};
	Coord2D					mCurrentMousePosition;
	static InputManagerC *sInstance;
};

void GetWorldCoordinates(int mouse_x, int mouse_y, float *gl_mouse_x, float *gl_mouse_y);

#endif