#pragma once
#include "ATB\AntTweakBar.h"
#include "Blayne_Keys.h"
#include "glm\glm.hpp"
#include "Blayne_Utilities.h"
//#include "Globals.h"

class Blayne_ATB
{
public:
	Blayne_ATB();

	bool Init();
	bool KeyboardCB(BLAYNE_KEY BlayneKey);

	bool PassiveMouseCB(int x, int y);

	void RenderSceneCB();

	bool MouseCB(BLAYNE_MOUSE Button, BLAYNE_KEY_STATE State, int x, int y);

private:

};

extern TwType TW_TYPE_BLAYNE_VECTOR3F;
extern TwType TW_TYPE_BLAYNE_VECTOR4F;
extern TwType TW_TYPE_BLAYNE_ATTENUATION;