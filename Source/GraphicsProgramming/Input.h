// Input class
// Stores current keyboard and mouse state include, pressed keys, mouse button pressed and mouse position.
// @author Paul Robertson
#ifndef INPUT_H
#define INPUT_H

class Input
{
	// Mouse struct stores mouse related data include cursor
	// x, y coordinates and left/right button pressed state.
	struct Mouse
	{
		int x,y;
		bool left, right;
	};

public:
	// Getters and setters for keys
	void SetKeyDown(unsigned char key);
	void SetKeyUp(unsigned char key);
	bool isKeyDown(int);

	// getters and setters for mouse buttons and position.
	void setMouseX(int);
	void setMouseY(int);
	void setMousePos(int x, int y);
	int getMouseX();
	int getMouseY();
	void setLeftMouseButton(bool b);
	bool isLeftMouseButtonPressed();

private:
	// Boolean array, element per key
	// Mouse struct object.
	bool keys[256];
	Mouse mouse;

};

#endif