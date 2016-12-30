#include "PickingTechnique.h"
#include <limits.h>
#include <string.h>
#include "Blayne_Utilities.h"
#include "Blayne_ATB.h"
#include <glm\gtc\type_ptr.hpp>

using namespace std;

PickingTechnique::PickingTechnique()
{
}

bool PickingTechnique::Init()
{
	if (!Technique::Init()) {
		printf("Failed to Init Picking Technique\n");
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/Picking.vertexshader")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/Picking.fragmentshader")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_PickingColourLocation = GetUniformLocation("PickingColor");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_PickingColourLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void PickingTechnique::SetWVP(const glm::mat4& WVP)
{
	//glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void PickingTechnique::SetWorldMatrix(const glm::mat4& WorldInverse)
{
	//glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, &WorldInverse[0][0]);
}

void PickingTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void PickingTechnique::SetPickingColour(const glm::vec4& PickingColour)
{
	glUniform4f(m_PickingColourLocation, PickingColour.x, PickingColour.y, PickingColour.z, PickingColour.w);
}