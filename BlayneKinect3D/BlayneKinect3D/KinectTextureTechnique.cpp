#include <limits.h>
#include <string.h>
#include "KinectTextureTechnique.h"
#include "Blayne_Utilities.h"
#include <glm\gtc\type_ptr.hpp>

KinectTextureTechnique::KinectTextureTechnique()
{
}

bool KinectTextureTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}
	if (!AddShader(GL_VERTEX_SHADER, "Shaders/SimpleVertexShader.vs")) {
		return false;
	}


	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/TextureFragmentShader.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_textureLocation = GetUniformLocation("myTextureSampler");

	// Error checking
	if (m_textureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void KinectTextureTechnique::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_textureLocation, TextureUnit);
}