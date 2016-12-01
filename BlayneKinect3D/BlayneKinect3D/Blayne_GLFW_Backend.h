#pragma once
#include "Blayne_Types.h"
#include "Blayne_Callbacks.h"

void GLFWBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);

void GLFWBackendTerminate();

bool GLFWBackendCreateWindow(Blayne_Types::uint Width, Blayne_Types::uint Height, bool isFullScreen, const char* pTitle);

void GLFWBackendRun(IBlayneCallbacks* pCallbacks);

void GLFWBackendSwapBuffers();

void GLFWBackendLeaveMainLoop();

void GLFWBackendSetMousePos(Blayne_Types::uint x, Blayne_Types::uint y);