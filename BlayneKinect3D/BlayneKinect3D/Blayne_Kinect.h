#pragma once
//some often used C standard library header files
#include <cstdlib>
#include <cstdio>
#include <cstring>

//some often used STL header files
#include <iostream>
#include <memory>
#include <vector>

#include <Kinect.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GLFW\glfw3.h>

#include "Blayne_Types.h"
#include "Blayne_Globals.h"
#include "Blayne_Technique.h"

// Depth buffer constants
#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424


//safe way of deleting a COM object
template<typename T>
void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

class BlayneKinect : public Technique
{
public:
	BlayneKinect();
	BlayneKinect(int windowWidth, int windowHeight);
	// Init the Quad we will be rendering our texture to.
	void InitRenderTarget();
	// Initialize our shaders
	bool InitShaders();
	// Take our colour buffer, and convert it to a texture
	bool ConvertColourBufferToTexture();

	// Init Kinect Buffers
	bool KinectInit();
	void Tick(float deltaTime);
	void Shutdown();
	
	void SetPixelBuffer(GLubyte* pixelBuffer) { m_pixelBuffer = pixelBuffer; }
	void PlotPixel(int x, int y,
		Blayne_Types::uint8 R,
		Blayne_Types::uint8 G,
		Blayne_Types::uint8 B,
		Blayne_Types::uint8 A);

	void processBodies(const unsigned int &bodyCount, IBody **bodies);	
	void DrawPixelBuffer();
	void DrawJoints();
	void DrawCircle(int x, int y, int radius, 
		Blayne_Types::uint8 R, 
		Blayne_Types::uint8 G, 
		Blayne_Types::uint8 B, 
		int _space);
private:
	int COLOUR_WIDTH, COLOUR_HEIGHT;
	int screenWidth, screenHeight;

	//void drawCircle(int x, int y, int radius, uint8 R, uint8 G, uint8 B, int _space);
	//pointer to buffer that contains pixels that get pushed to the screen
	//size of this buffer is SCRWIDTH * SCRWIDTH * sizeof(uint32)
	GLubyte* m_pixelBuffer = nullptr;
	GLFWwindow* screen;
	IKinectSensor* m_sensor = nullptr;
	IDepthFrameReader* m_depthFrameReader = nullptr;

	Blayne_Types::uint16* m_depthBuffer = nullptr;

	int m_depthWidth = 0, m_depthHeight = 0;

	// Colour, similar as depth
	IColorFrameReader* m_colorFrameReader = nullptr;
	//uint32* m_colorBuffer = nullptr;
	GLubyte* m_colorBuffer = nullptr;
	ICoordinateMapper* m_coordinateMapper = nullptr;
	ColorSpacePoint* m_colorSpacePoints = nullptr;
	IBodyFrameReader* m_bodyFrameReader = nullptr;

	// Our vertex buffer for our quad.
	GLuint m_quadVertexBuffer;
	// Our texture
	GLuint m_tex;
	// Uniform location for texture
	GLuint m_texID;
};