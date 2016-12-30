#pragma once
#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Blayne_Types.h"
#include "Blayne_Technique.h"
#include "Blayne_Lights_Common.h"


class PickingTechnique : public Technique {
public:
	PickingTechnique();

	virtual bool Init();

	void SetWVP(const glm::mat4& WVP);
	void SetWorldMatrix(const glm::mat4& WVP);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
	void SetPickingColour(const glm::vec4& PickingColour);
private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_PickingColourLocation;
};