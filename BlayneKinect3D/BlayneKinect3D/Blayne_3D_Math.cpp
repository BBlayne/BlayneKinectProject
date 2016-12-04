#include "Blayne_3D_Math.h"

glm::mat4 Blayne_3D_Math::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
	return glm::scale(glm::vec3(ScaleX, ScaleY, ScaleZ));
}

glm::mat4 Blayne_3D_Math::InitRotateTransform(float RotateX, float RotateY, float RotateZ) {
	return glm::eulerAngleXYZ(RotateX, RotateY, RotateZ);
}

glm::mat4 Blayne_3D_Math::InitRotateTransform(const glm::quat& _quat) {
	return glm::mat4_cast(_quat);
}

glm::mat4 Blayne_3D_Math::InitTranslationTransform(float x, float y, float z) {
	return glm::translate(glm::vec3(x, y, z));
}

glm::mat4 Blayne_3D_Math::InitCameraTransform(const glm::vec3& Position, const glm::vec3& LookDir, const glm::vec3& Up) {
	glm::vec3 N = glm::normalize(LookDir);
	glm::vec3 U = glm::normalize(Up);
	U = glm::cross(U, N);
	glm::vec3 V = glm::cross(N, U);

	// My Instincts.
	return glm::lookAt(Position, 
						LookDir, 
							Up);
}

glm::mat4 Blayne_3D_Math::InitPersProjTransform(const PersProjInfo& p) {
	return glm::perspective(p.FOV, p.aspect_ratio, p.zNear, p.zFar);
}

glm::mat4 Blayne_3D_Math::InitOrthoProjTransform(const OrthoProjInfo& p) {
	// HRM? Might be p.l / 2 etc etc etc etc etc
	return glm::ortho(p.l, p.r, p.b, p.t, p.n, p.f);
}


aiMatrix4x4 Blayne_3D_Math::fromMatrix3x3(const aiMatrix3x3& AssimpMatrix)
{
	aiMatrix4x4 m(AssimpMatrix);
	return m;
}

void Blayne_3D_Math::CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to)
{
	to[0][0] = from->a1; to[1][0] = from->a2;
	to[2][0] = from->a3; to[3][0] = from->a4;
	to[0][1] = from->b1; to[1][1] = from->b2;
	to[2][1] = from->b3; to[3][1] = from->b4;
	to[0][2] = from->c1; to[1][2] = from->c2;
	to[2][2] = from->c3; to[3][2] = from->c4;
	to[0][3] = from->d1; to[1][3] = from->d2;
	to[2][3] = from->d3; to[3][3] = from->d4;
}

void Blayne_3D_Math::CopyGlMatToAiMat(const glm::mat4 from, aiMatrix4x4 &to)
{
	to[0][0] = from[0][0]; to[0][1] = from[1][0]; to[0][2] = from[2][0]; to[0][3] = from[3][0];
	to[1][0] = from[0][1]; to[1][1] = from[1][1]; to[1][2] = from[2][1]; to[1][3] = from[3][1];
	to[2][0] = from[0][2]; to[2][1] = from[1][2]; to[2][2] = from[2][2]; to[2][3] = from[3][2];
	to[3][0] = from[0][3]; to[3][1] = from[1][3]; to[3][2] = from[2][3]; to[3][3] = from[3][3];
}

void Blayne_3D_Math::PrintMatrix(glm::mat4 matrix)
{

}

void Blayne_3D_Math::PrintAiMatrix(aiMatrix4x4 matrix)
{

}

void Blayne_3D_Math::PrintVector4(glm::vec4 _vector)
{

}

void Blayne_3D_Math::PrintVector3(glm::vec3 _vector)
{

}