#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Blayne_Globals.h"
#include "Blayne_Technique.h"

class RenderToTexture : public Technique {
public:
	bool InitFrameBuffer(int windowWidth, int windowHeight, GLuint _tex);
	bool InitShaders();
	void Render(GLuint _tex);
	void RenderToFrameBuffer();
	void RenderToScreen();
private:
	GLuint m_VAO = 0;
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	// The texture we're going to render to
	GLuint renderedTexture;
	// The depth buffer
	GLuint depthrenderbuffer;
	GLuint quad_vertexbuffer;
	// Create and compile our GLSL program from the shaders
	GLuint quad_programID;
	GLuint texID;
	GLuint timeID;
	// window dimensions
	int windowWidth, windowHeight;
};