#include "BlayneSkinningTechnique.h"
#include <limits.h>
#include <string.h>
#include "Blayne_Utilities.h"
#include "Blayne_ATB.h"
#include <glm\gtc\type_ptr.hpp>

using namespace std;

SkinningTechnique::SkinningTechnique()
{
}

bool SkinningTechnique::Init()
{
	if (!Technique::Init()) {
		printf("Failed to Init SkinningTechnique\n");
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/skinning.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/skinning.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_colorTextureLocation = GetUniformLocation("gColorMap");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");

	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
		m_colorTextureLocation == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
		m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		m_spotLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		m_spotLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = GetUniformLocation(Name);
	}

	return true;
}

void SkinningTechnique::SetWVP(const glm::mat4& WVP)
{
	//glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}


void SkinningTechnique::SetWorldMatrix(const glm::mat4& WorldInverse)
{
	//glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, &WorldInverse[0][0]);
}


void SkinningTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureLocation, TextureUnit);
}


void SkinningTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = glm::normalize(Light.Direction);
	//Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}


void SkinningTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}


void SkinningTechnique::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}


void SkinningTechnique::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}


void SkinningTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void SkinningTechnique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(m_numSpotLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glm::vec3 Direction = glm::normalize(pLights[i].Direction);
		//Direction.Normalize();
		glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
		//glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
		glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].Cutoff)));
		glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_spotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_spotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void SkinningTechnique::SetBoneTransform(Blayne_Types::uint32 index, const glm::mat4& transform)
{
	assert(Index < MAX_BONES);
	//Transform.Print();
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_FALSE, &transform[0][0]);
}