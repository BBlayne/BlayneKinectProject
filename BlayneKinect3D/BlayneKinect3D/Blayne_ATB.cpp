#include "Blayne_ATB.h"
#include "Blayne_Lights_Common.h"

TwType TW_TYPE_BLAYNE_VECTOR3F;
TwType TW_TYPE_BLAYNE_VECTOR4F;
TwType TW_TYPE_BLAYNE_ATTENUATION;

Blayne_ATB::Blayne_ATB()
{

}

bool Blayne_ATB::Init()
{
	bool ret = false;

	if (TwInit(TW_OPENGL_CORE, NULL) == 1)
	{
		TwStructMember Vector3fMembers[] = {
			{ "x", TW_TYPE_FLOAT, offsetof(glm::vec3, x), "" },
			{ "y", TW_TYPE_FLOAT, offsetof(glm::vec3, y), "" },
			{ "z", TW_TYPE_FLOAT, offsetof(glm::vec3, z), "" }
		};

		TW_TYPE_BLAYNE_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(glm::vec3), NULL, NULL);

		TwStructMember Vector4fMembers[] = {
			{ "x", TW_TYPE_FLOAT, offsetof(glm::vec4, x), "" },
			{ "y", TW_TYPE_FLOAT, offsetof(glm::vec4, y), "" },
			{ "z", TW_TYPE_FLOAT, offsetof(glm::vec4, z), "" },
			{ "w", TW_TYPE_FLOAT, offsetof(glm::vec4, w), "" }
		};

		TW_TYPE_BLAYNE_VECTOR4F = TwDefineStruct("Vector4f", Vector4fMembers, 4, sizeof(glm::vec4), NULL, NULL);

		TwStructMember AttenuationMembers[] = {
			{ "Const", TW_TYPE_FLOAT, offsetof(LightAttenuation, Constant), "" },
			{ "Linear", TW_TYPE_FLOAT, offsetof(LightAttenuation, Linear), "" },
			{ "Exp", TW_TYPE_FLOAT, offsetof(LightAttenuation, Exp), "" }
		};

		TW_TYPE_BLAYNE_ATTENUATION = TwDefineStruct("Attenuation", AttenuationMembers, 3, sizeof(LightAttenuation), NULL, NULL);

		ret = true;
	}

	
	return ret;
}

static int BlayneKeyToATBKey(BLAYNE_KEY BlayneKey)
{
	if (BlayneKey >= BLAYNE_KEY_SPACE && BlayneKey <= BLAYNE_KEY_RIGHT_BRACKET) {
		return BlayneKey;
	}

	switch (BlayneKey) {
		case BLAYNE_KEY_BACKSPACE:
			return TW_KEY_BACKSPACE;
		case BLAYNE_KEY_TAB:
			return TW_KEY_TAB;
			//            return TW_KEY_CLEAR;
		case BLAYNE_KEY_ENTER:
			return TW_KEY_RETURN;

			//return TW_KEY_PAUSE;
		case BLAYNE_KEY_ESCAPE:
			return TW_KEY_ESCAPE;
		case BLAYNE_KEY_DELETE:
			return TW_KEY_DELETE;
		case BLAYNE_KEY_UP:
			return TW_KEY_UP;
		case BLAYNE_KEY_DOWN:
			return TW_KEY_DOWN;
		case BLAYNE_KEY_RIGHT:
			return TW_KEY_RIGHT;
		case BLAYNE_KEY_LEFT:
			return TW_KEY_LEFT;
		case BLAYNE_KEY_INSERT:
			return TW_KEY_INSERT;
		case BLAYNE_KEY_HOME:
			return TW_KEY_HOME;
		case BLAYNE_KEY_END:
			return TW_KEY_END;
		case BLAYNE_KEY_PAGE_UP:
			return TW_KEY_PAGE_UP;
		case BLAYNE_KEY_PAGE_DOWN:
			return TW_KEY_PAGE_DOWN;
		case BLAYNE_KEY_F1:
			return TW_KEY_F1;
		case BLAYNE_KEY_F2:
			return TW_KEY_F2;
		case BLAYNE_KEY_F3:
			return TW_KEY_F3;
		case BLAYNE_KEY_F4:
			return TW_KEY_F4;
		case BLAYNE_KEY_F5:
			return TW_KEY_F5;
		case BLAYNE_KEY_F6:
			return TW_KEY_F6;
		case BLAYNE_KEY_F7:
			return TW_KEY_F7;
		case BLAYNE_KEY_F8:
			return TW_KEY_F8;
		case BLAYNE_KEY_F9:
			return TW_KEY_F9;
		case BLAYNE_KEY_F10:
			return TW_KEY_F10;
		case BLAYNE_KEY_F11:
			return TW_KEY_F11;
		case BLAYNE_KEY_F12:
			return TW_KEY_F12;
		default:
			OGLDEV_ERROR("Unimplemented OGLDEV to ATB key");
	}

	return TW_KEY_LAST;
}

bool Blayne_ATB::KeyboardCB(BLAYNE_KEY BlayneKey)
{
	int ATBKey = BlayneKeyToATBKey(BlayneKey);

	if (ATBKey == TW_KEY_LAST) {
		return false;
	}

	return (TwKeyPressed(ATBKey, TW_KMOD_NONE) == 1);
}

bool Blayne_ATB::PassiveMouseCB(int x, int y)
{
	return (TwMouseMotion(x, y) == 1);
}


bool Blayne_ATB::MouseCB(BLAYNE_MOUSE Button, BLAYNE_KEY_STATE State, int x, int y)
{
	TwMouseButtonID btn = (Button == BLAYNE_MOUSE_BUTTON_LEFT) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (State == BLAYNE_KEY_STATE_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;

	return (TwMouseButton(ma, btn) == 1);
}

void Blayne_ATB::RenderSceneCB()
{
	// ???
}