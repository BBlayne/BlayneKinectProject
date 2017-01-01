#include "Blayne_3D_Math.h"
#include<glm/gtc/quaternion.hpp>
#include<glm\gtx\quaternion.hpp>
#include<glm/common.hpp>
#include <glm\gtc\type_ptr.hpp>

glm::vec3 Blayne_3D_Math::get_arcball_vector(int x, int y, int WIDTH, int HEIGHT)
{
	glm::vec3 P = glm::vec3(1.0*x / WIDTH * 2 - 1.0,
		1.0*y / HEIGHT * 2 - 1.0,
		0);
	P.y = -P.y;
	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= 1 * 1)
		P.z = sqrt(1 * 1 - OP_squared);  // Pythagore
	else
		P = glm::normalize(P);  // nearest point
	return P;
}

glm::mat4 Blayne_3D_Math::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
	return glm::scale(glm::vec3(ScaleX, ScaleY, ScaleZ));
}

glm::mat4 Blayne_3D_Math::InitRotateTransform(float RotateX, float RotateY, float RotateZ) {
	glm::quat myQuat;
	glm::vec3 EulerAngles = glm::vec3(RotateX, RotateY, RotateZ);
	myQuat = glm::quat(EulerAngles);
	//return glm::toMat4(myQuat);

	return glm::rotate(glm::mat4(1.0), glm::radians(RotateX), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0), glm::radians(RotateY), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0), glm::radians(RotateZ), glm::vec3(0, 0, 1));

	//return glm::eulerAngleXYZ(RotateX, RotateY, RotateZ);
	//return glm::toMat4(glm::quat(1, RotateX, RotateY, RotateZ));
	
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

glm::quat Blayne_3D_Math::aiQuatToGlMQuat(aiQuaternion _quat)
{
	glm::quat temp;
	temp.w = _quat.w;
	temp.x = _quat.x;
	temp.y = _quat.y;
	temp.z = _quat.z;

	return temp;
}

glm::vec3 Blayne_3D_Math::aiVec3ToGlMVec3(aiVector3D _vec)
{
	glm::vec3 temp;
	temp.x = _vec.x;
	temp.y = _vec.y;
	temp.z = _vec.z;

	return temp;
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

void Blayne_3D_Math::PrintQuat(glm::quat _vector)
{
	printf("W: %.3f, X: %.3f, Y: %.3f, Z: %.3f. \n", _vector.w,
		_vector.x, _vector.y, _vector.z);
}