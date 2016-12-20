#pragma once
#include "Blayne_Lights_Common.h"
#include "Blayne_Technique.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class KinectTextureTechnique : public Technique {
public:
	KinectTextureTechnique();
	virtual bool Init();
	void SetTextureUnit(unsigned int TextureUnit);
private:
	GLuint m_textureLocation;
};