#pragma once
#include "Blayne_Keys.h"

class IBlayneCallbacks
{
public:
	virtual void KeyboardCB(BLAYNE_KEY BlayneKey, BLAYNE_KEY_STATE BlayneKeyState = BLAYNE_KEY_STATE_PRESS) {};
	virtual void PassiveMouseHandlerCB(int x, int y) {};
	virtual void RenderSceneCB() {};
	virtual void MouseClickHandlerCB(BLAYNE_MOUSE Button, BLAYNE_KEY_STATE State, int x, int y) {};
};