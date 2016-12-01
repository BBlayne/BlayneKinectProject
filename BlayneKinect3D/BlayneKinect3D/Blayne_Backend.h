#pragma once
#include "Blayne_Types.h"
#include "Blayne_Callbacks.h"

enum BLAYNE_BACKEND_TYPE {
	BLAYNE_BACKEND_TYPE_GLUT,
	BLAYNE_BACKEND_TYPE_GLFW
};

void BlayneBackendInit(BLAYNE_BACKEND_TYPE BackendType, int argc, char** argv, bool WithDepth, bool WithStencil);

void BlayneBackendTerminate();

bool BlayneBackendCreateWindow(Blayne_Types::uint Width, Blayne_Types::uint Height, bool isFullScreen, const char* pTitle);

void BlayneBackendRun(IBlayneCallbacks* pCallbacks);

void BlayneBackendLeaveMainLoop();

void BlayneBackendSwapBuffers();

void BlayneBackendSetMousePos(Blayne_Types::uint x, Blayne_Types::uint y);