//#include "GL\glew.h"
//#include "GLFW\glfw3.h"
//#include "GL\freeglut.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include <stdio.h>

#include "GL\glew.h"
//#define GLFW_DLL
#include "GLFW\glfw3.h"

#include "Blayne_Utilities.h"
#include "Blayne_GLFW_Backend.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static IBlayneCallbacks* s_pCallbacks = NULL;

static bool sWithDepth = false;
static bool sWithStencil = false;
static GLFWwindow* s_pWindow = NULL;

static BLAYNE_KEY GLFWKeyToBLAYNEKey(Blayne_Types::uint32 Key)
{
	if (Key >= GLFW_KEY_SPACE && Key <= GLFW_KEY_RIGHT_BRACKET) {
		return (BLAYNE_KEY)Key;
	}

	switch (Key) {
	case GLFW_KEY_ESCAPE:
		return BLAYNE_KEY_ESCAPE;
	case GLFW_KEY_ENTER:
		return BLAYNE_KEY_ENTER;
	case GLFW_KEY_TAB:
		return BLAYNE_KEY_TAB;
	case GLFW_KEY_BACKSPACE:
		return BLAYNE_KEY_BACKSPACE;
	case GLFW_KEY_INSERT:
		return BLAYNE_KEY_INSERT;
	case GLFW_KEY_DELETE:
		return BLAYNE_KEY_DELETE;
	case GLFW_KEY_RIGHT:
		return BLAYNE_KEY_RIGHT;
	case GLFW_KEY_LEFT:
		return BLAYNE_KEY_LEFT;
	case GLFW_KEY_DOWN:
		return BLAYNE_KEY_DOWN;
	case GLFW_KEY_UP:
		return BLAYNE_KEY_UP;
	case GLFW_KEY_PAGE_UP:
		return BLAYNE_KEY_PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:
		return BLAYNE_KEY_PAGE_DOWN;
	case GLFW_KEY_HOME:
		return BLAYNE_KEY_HOME;
	case GLFW_KEY_END:
		return BLAYNE_KEY_END;
	case GLFW_KEY_F1:
		return BLAYNE_KEY_F1;
	case GLFW_KEY_F2:
		return BLAYNE_KEY_F2;
	case GLFW_KEY_F3:
		return BLAYNE_KEY_F3;
	case GLFW_KEY_F4:
		return BLAYNE_KEY_F4;
	case GLFW_KEY_F5:
		return BLAYNE_KEY_F5;
	case GLFW_KEY_F6:
		return BLAYNE_KEY_F6;
	case GLFW_KEY_F7:
		return BLAYNE_KEY_F7;
	case GLFW_KEY_F8:
		return BLAYNE_KEY_F8;
	case GLFW_KEY_F9:
		return BLAYNE_KEY_F9;
	case GLFW_KEY_F10:
		return BLAYNE_KEY_F10;
	case GLFW_KEY_F11:
		return BLAYNE_KEY_F11;
	case GLFW_KEY_F12:
		return BLAYNE_KEY_F12;
	default:
		OGLDEV_ERROR("Unimplemented OGLDEV key");
	}

	return BLAYNE_KEY_UNDEFINED;
}

static BLAYNE_MOUSE GLFWMouseToBlayneMouse(Blayne_Types::uint32 Button)
{
	switch (Button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		return BLAYNE_MOUSE_BUTTON_LEFT;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return BLAYNE_MOUSE_BUTTON_RIGHT;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return BLAYNE_MOUSE_BUTTON_MIDDLE;
	default:
		OGLDEV_ERROR("Unimplemented OGLDEV mouse button");
	}

	return BLAYNE_MOUSE_UNDEFINED;
}

static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	BLAYNE_KEY BlayneKey = GLFWKeyToBLAYNEKey(key);
	BLAYNE_KEY_STATE BlayneKeyState = (action == GLFW_PRESS) ? BLAYNE_KEY_STATE_PRESS : BLAYNE_KEY_STATE_RELEASE;
	s_pCallbacks->KeyboardCB(BlayneKey, BlayneKeyState);
}


static void CursorPosCallback(GLFWwindow* pWindow, double x, double y)
{
	s_pCallbacks->PassiveMouseHandlerCB((int)x, (int)y);
}


static void MouseButtonCallback(GLFWwindow* pWindow, int Button, int Action, int Mode)
{
	BLAYNE_MOUSE BlayneMouse = GLFWMouseToBlayneMouse(Button);

	BLAYNE_KEY_STATE State = (Action == GLFW_PRESS) ? BLAYNE_KEY_STATE_PRESS : BLAYNE_KEY_STATE_RELEASE;

	double x, y;

	glfwGetCursorPos(pWindow, &x, &y);

	s_pCallbacks->MouseClickHandlerCB(BlayneMouse, State, (int)x, (int)y);
}

static void InitCallbacks()
{
	glfwSetKeyCallback(s_pWindow, KeyCallback);
	glfwSetCursorPosCallback(s_pWindow, CursorPosCallback);
	glfwSetMouseButtonCallback(s_pWindow, MouseButtonCallback);
}

void GLFWErrorCallback(int error, const char* description)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "GLFW error %d - %s", error, description);
	MessageBoxA(NULL, msg, NULL, 0);
#else
	fprintf(stderr, "GLFW error %d - %s", error, description);
#endif    
	exit(0);
}


void GLFWBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil)
{
	sWithDepth = WithDepth;
	sWithStencil = WithStencil;

	glfwSetErrorCallback(GLFWErrorCallback);

	if (glfwInit() != 1) {
		OGLDEV_ERROR("Error initializing GLFW");
		exit(1);
	}

	int Major, Minor, Rev;

	glfwGetVersion(&Major, &Minor, &Rev);

	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);
}


void GLFWBackendTerminate()
{
	glfwDestroyWindow(s_pWindow);	
	glfwTerminate();
}


bool GLFWBackendCreateWindow(Blayne_Types::uint32 Width, Blayne_Types::uint32 Height, bool isFullScreen, const char* pTitle)
{
	GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;

	s_pWindow = glfwCreateWindow(Width, Height, pTitle, pMonitor, NULL);

	if (!s_pWindow) {
		OGLDEV_ERROR("error creating window");
		exit(1);
	}

	glfwMakeContextCurrent(s_pWindow);

	// Must be done after glfw is initialized!
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		OGLDEV_ERROR((const char*)glewGetErrorString(res));
		exit(1);
	}

	return (s_pWindow != NULL);
}

void GLFWBackendRun(IBlayneCallbacks* pCallbacks)
{
	if (!pCallbacks) {
		OGLDEV_ERROR("callbacks not specified");
		exit(1);
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	if (sWithDepth) {
		glEnable(GL_DEPTH_TEST);
	}

	s_pCallbacks = pCallbacks;
	InitCallbacks();

	while (!glfwWindowShouldClose(s_pWindow)) {
		s_pCallbacks->RenderSceneCB();
		glfwSwapBuffers(s_pWindow);
		glfwPollEvents();
	}
}


void GLFWBackendSwapBuffers()
{
	// Nothing to do here
}


void GLFWBackendLeaveMainLoop()
{
	glfwSetWindowShouldClose(s_pWindow, 1);
}


void GLFWBackendSetMousePos(Blayne_Types::uint32 x, Blayne_Types::uint32 y)
{
	glfwSetCursorPos(s_pWindow, (double)x, (double)y);
}