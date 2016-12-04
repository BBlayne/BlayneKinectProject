#include "Blayne_Pipeline.h"

// Remember all matrix transformations are in reverse order!
// Returns the Projection matrix
const glm::mat4& Blayne_Pipeline::GetProjTrans()
{
	m_ProjTransformation = Blayne_3D_Math::InitPersProjTransform(m_persProjInfo);
	return m_ProjTransformation;
}

// View * Projection Matrix
const glm::mat4& Blayne_Pipeline::GetVPTrans()
{
	//GetViewTrans();
	//GetProjTrans();

	
	//m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	m_VPtransformation = GetProjTrans() * GetViewTrans();
	return m_VPtransformation;
}

// Returns the WorldSpace transformation matrix
const glm::mat4& Blayne_Pipeline::GetWorldTrans()
{
	glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;

	ScaleTrans = Blayne_3D_Math::InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateTrans = Blayne_3D_Math::InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	RotateTrans = glm::mat4(1.0f);
	TranslationTrans = Blayne_3D_Math::InitTranslationTransform(m_translation.x, m_translation.y, m_translation.z);

	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	return m_Wtransformation;
}

// Returns the Camera Space transformation matrix
const glm::mat4& Blayne_Pipeline::GetViewTrans()
{
	glm::mat4 CameraTranslationTrans, CameraRotateTrans;

	CameraTranslationTrans = Blayne_3D_Math::InitTranslationTransform(m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
	CameraRotateTrans = Blayne_3D_Math::InitCameraTransform(m_Camera.Pos, m_Camera.LookDir, m_Camera.Up);

	//m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;
	m_Vtransformation = CameraRotateTrans;

	return m_Vtransformation;
}

// World View projection matrix
const glm::mat4& Blayne_Pipeline::GetWVPTrans()
{
	//GetWorldTrans();
	//GetVPTrans();

	//m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	m_WVPtransformation = GetVPTrans() * GetWorldTrans();
	return m_WVPtransformation;
}


// Returns us the MVP Matrix where our projection is orthographic
const glm::mat4& Blayne_Pipeline::GetWVOrthoPTrans()
{
	//GetWorldTrans();
	//GetViewTrans();

	glm::mat4 Projection;
	Projection = Blayne_3D_Math::InitOrthoProjTransform(m_orthoProjInfo);

	//m_WVPtransformation = Projection * m_Vtransformation * m_Wtransformation;
	m_WVPtransformation = Projection * GetViewTrans() * GetWorldTrans();
	return m_WVPtransformation;
}

// Worldspace * ViewSpace Matrix
const glm::mat4& Blayne_Pipeline::GetWVTrans()
{
	//GetWorldTrans();
	//GetViewTrans();

	//m_WVtransformation = m_Vtransformation * m_Wtransformation;
	m_WVtransformation = GetViewTrans() * GetWorldTrans();
	return m_WVtransformation;
}

// Worldspace * Projection transformation matrix
const glm::mat4& Blayne_Pipeline::GetWPTrans()
{
	glm::mat4 PersProjTrans;

	//GetWorldTrans();
	PersProjTrans = Blayne_3D_Math::InitPersProjTransform(m_persProjInfo);

	//m_WPtransformation = PersProjTrans * m_Wtransformation;
	m_WPtransformation = PersProjTrans * GetWorldTrans();
	return m_WPtransformation;
}