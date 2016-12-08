#include "BlayneRenderToTexture.h"

bool RenderToTexture::InitFrameBuffer(int _windowWidth, int _windowHeight, GLuint _tex)
{
	this->windowWidth = _windowWidth;
	this->windowHeight = _windowHeight;

	//_tex = renderedTexture;
	// Create the VAO
	//glGenVertexArrays(1, &m_VAO);
	//glBindVertexArray(m_VAO);

	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------
	//FramebufferName = 0;
	//glGenFramebuffers(1, &FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	//glGenTextures(1, &renderedTexture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->windowWidth, this->windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glGenRenderbuffers(1, &depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->windowWidth, this->windowHeight);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _tex, 0);

	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

								   // Always check that our framebuffer is ok
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	return false;

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Make sure the VAO is not changed from the outside
	//glBindVertexArray(0);

	return true;
}

bool RenderToTexture::InitShaders()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/Passthrough.vertexshader")) {
		return false;
	}


	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/BasicTexture.fragmentshader")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	//m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);
	texID = GetUniformLocation("renderedTexture");
	timeID = GetUniformLocation("time");

	return true;
}

void RenderToTexture::Render(GLuint _tex)
{

	//_tex = renderedTexture;
	glBindVertexArray(m_VAO);
	// Render on the whole framebuffer, complete from the lower left corner to the upper right
	glViewport(0,0,windowWidth,windowHeight);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	//glUseProgram(quad_programID);
	this->Enable();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(texID, 0);

	glUniform1f(timeID, (float)(glfwGetTime()*10.0f) );

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);

	// Make sure the VAO is not changed from the outside    
	//glBindVertexArray(0);
	
}

// Needs to not be called in order to render the scene normally.
// If it is being called then there is a black screen.
void RenderToTexture::RenderToFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
}

// So far based on explorations, if RenderToFrameBuffer is enabled,
// then RenderToScreen needs to be abled. Otherwise black screen.
void RenderToTexture::RenderToScreen()
{
	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}