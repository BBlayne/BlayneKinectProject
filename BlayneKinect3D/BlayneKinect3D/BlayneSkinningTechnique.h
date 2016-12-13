#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Blayne_Types.h"
#include "Blayne_Technique.h"
#include "Blayne_Lights_Common.h"


class SkinningTechnique : public Technique {
public:
	static const Blayne_Types::uint32 MAX_POINT_LIGHTS = 2;
	static const Blayne_Types::uint32 MAX_SPOT_LIGHTS = 2;
	static const Blayne_Types::uint32 MAX_BONES = 100;

	SkinningTechnique();

	virtual bool Init();

	void SetWVP(const glm::mat4& WVP);
	void SetWorldMatrix(const glm::mat4& WVP);
	void SetColorTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void SetBoneTransform(Blayne_Types::uint32 index, const glm::mat4& transform);
private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_colorTextureLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];

	GLuint m_boneLocation[MAX_BONES];
};