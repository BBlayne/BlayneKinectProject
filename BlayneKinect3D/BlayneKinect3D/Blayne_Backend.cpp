#include "Blayne_Backend.h"
#include <ATB\AntTweakBar.h>
#include "Blayne_GLFW_Backend.h"
#include "Blayne_Utilities.h"

static BLAYNE_BACKEND_TYPE sBackendType = BLAYNE_BACKEND_TYPE_GLFW;

void BlayneBackendInit(BLAYNE_BACKEND_TYPE BackendType, int argc, char** argv, bool WithDepth, bool WithStencil)
{
	sBackendType = BackendType;

	switch (BackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	GLUTBackendInit(argc, argv, WithDepth, WithStencil);
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendInit(argc, argv, WithDepth, WithStencil);
		break;
	default:
		assert(0);
	}
}


void BlayneBackendTerminate()
{
	switch (sBackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	// nothing to do here...
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendTerminate();
		break;
	default:
		assert(0);
	}
}


bool BlayneBackendCreateWindow(Blayne_Types::uint32 Width, Blayne_Types::uint32 Height, bool isFullScreen, const char* pTitle)
{
	TwWindowSize(Width, Height);

	switch (sBackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	return GLUTBackendCreateWindow(Width, Height, isFullScreen, pTitle);
	case BLAYNE_BACKEND_TYPE_GLFW:
		return GLFWBackendCreateWindow(Width, Height, isFullScreen, pTitle);
	default:
		assert(0);
	}



	return false;
}


void BlayneBackendRun(IBlayneCallbacks* pCallbacks)
{
	switch (sBackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	GLUTBackendRun(pCallbacks);
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendRun(pCallbacks);
		break;
	default:
		assert(0);
	}
}


void BlayneBackendSwapBuffers()
{
	TwDraw();

	switch (sBackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	GLUTBackendSwapBuffers();
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendSwapBuffers();
		break;
	default:
		assert(0);
	}
}


void BlayneBackendLeaveMainLoop()
{
	switch (sBackendType) {
	//case BLAYNE_BACKEND_TYPE_GLUT:
	//	GLUTBackendLeaveMainLoop();
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendLeaveMainLoop();
		break;
	default:
		assert(0);
	}
}


void BlayneBackendSetMousePos(Blayne_Types::uint32 x, Blayne_Types::uint32 y)
{
	switch (sBackendType) {
	//case OGLDEV_BACKEND_TYPE_GLUT:
		//GLUTBackendLeaveMainLoop();
	//	break;
	case BLAYNE_BACKEND_TYPE_GLFW:
		GLFWBackendSetMousePos(x, y);
		break;
	default:
		assert(0);
	}
}