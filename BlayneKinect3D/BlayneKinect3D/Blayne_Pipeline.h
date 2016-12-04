#pragma once
#include "Blayne_Camera.h"
#include "Blayne_3D_Math.h"

struct Orientation
{
	glm::vec3 m_scale;
	glm::vec3 m_rotation; // Quaternion?
	glm::vec3 m_translation; // Pos?

	Orientation()
	{
		m_scale =	    glm::vec3(1.0f, 1.0f, 1.0f);
		m_rotation =    glm::vec3(0.0f, 0.0f, 0.0f);
		m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

// Appears to be a utility class for computing matrix transformations.
class Blayne_Pipeline
{
public:
	Blayne_Pipeline()
	{
		m_scale =		glm::vec3(1.0f, 1.0f, 1.0f);
		m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo =  glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Scale(float s)
	{
		Scale(s, s, s);
	}

	void Scale(const glm::vec3& scale)
	{
		Scale(scale.x, scale.y, scale.z);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_translation.x = x;
		m_translation.y = y;
		m_translation.z = z;
	}

	void WorldPos(const glm::vec3& _Pos)
	{
		m_translation = _Pos;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}

	void Rotate(const glm::vec3& r)
	{
		Rotate(r.x, r.y, r.z);
	}

	void SetPerspectiveProj(const PersProjInfo& p)
	{
		m_persProjInfo = p;
	}

	void SetOrthographicProj(const OrthoProjInfo& p)
	{
		m_orthoProjInfo = p;
	}

	void SetCamera(const glm::vec3& Pos, const glm::vec3& _LookDir, const glm::vec3& Up)
	{
		m_Camera.Pos = Pos;
		m_Camera.LookDir = _LookDir;
		m_Camera.Up = Up;
	}

	void SetCamera(const Blayne_Camera& camera)
	{
		SetCamera(camera.GetCameraPosition(), camera.GetCameraLookDirection(), camera.GetCameraUpDirection());
	}

	void Orient(const Orientation& o)
	{
		m_scale = o.m_scale;
		m_translation = o.m_translation;
		m_rotateInfo = o.m_rotation;
	}

	const glm::mat4& GetWPTrans();
	const glm::mat4& GetWVTrans();
	const glm::mat4& GetVPTrans();
	const glm::mat4& GetWVPTrans();
	const glm::mat4& GetWVOrthoPTrans();
	const glm::mat4& GetWorldTrans();
	const glm::mat4& GetViewTrans();
	const glm::mat4& GetProjTrans();

private:
	glm::vec3 m_scale;
	glm::vec3 m_translation;
	glm::vec3 m_rotateInfo;

	PersProjInfo m_persProjInfo;
	OrthoProjInfo m_orthoProjInfo;

	struct {
		glm::vec3 Pos;
		glm::vec3 LookDir;
		glm::vec3 Up;
	} m_Camera;

	glm::mat4 m_WVPtransformation; // World View Projection
	glm::mat4 m_VPtransformation;// View Projection
	glm::mat4 m_WPtransformation; // World Projection
	glm::mat4 m_WVtransformation; // World View
	glm::mat4 m_Wtransformation; // World
	glm::mat4 m_Vtransformation; // View
	glm::mat4 m_ProjTransformation; // Projection
};