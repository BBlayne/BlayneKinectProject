#pragma once
#include "Blayne_Keys.h"

class IBlayneCallbacks
{
public:
	virtual void KeyboardCB(BLAYNE_KEY OgldevKey, BLAYNE_KEY_STATE OgldevKeyState = BLAYNE_KEY_STATE_PRESS) {};
	virtual void PassiveMouseCB(int x, int y) {};
	virtual void RenderSceneCB() {};
	virtual void MouseCB(BLAYNE_MOUSE Button, BLAYNE_KEY_STATE State, int x, int y) {};
};