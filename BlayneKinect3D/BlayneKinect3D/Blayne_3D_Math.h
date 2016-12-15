#pragma once
#include "assimp\Importer.hpp"
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float aspect_ratio;
	float zNear;
	float zFar;
};

struct OrthoProjInfo
{
	float r;        // right
	float l;        // left
	float b;        // bottom
	float t;        // top
	float n;        // z near
	float f;        // z far
};

class Blayne_3D_Math {
public:
	static aiMatrix4x4 fromMatrix3x3(const aiMatrix3x3& AssimpMatrix);
	static void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to);
	static void CopyGlMatToAiMat(const glm::mat4 from, aiMatrix4x4 &to);
	static void PrintMatrix(glm::mat4 matrix);
	static void PrintAiMatrix(aiMatrix4x4 matrix);
	static void PrintVector4(glm::vec4 _vector);
	static void PrintVector3(glm::vec3 _vector);
	static void PrintQuat(glm::quat _vector);
	static glm::quat aiQuatToGlMQuat(aiQuaternion _quat);
	static glm::vec3 aiVec3ToGlMVec3(aiVector3D _vec);
	static glm::mat4 InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	static glm::mat4 InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	static glm::mat4 InitRotateTransform(const glm::quat& quat);
	static glm::mat4 InitTranslationTransform(float x, float y, float z);
	static glm::mat4 InitCameraTransform(const glm::vec3& Position, const glm::vec3& LookAt, const glm::vec3& Up);
	static glm::mat4 InitPersProjTransform(const PersProjInfo& p);
	static glm::mat4 InitOrthoProjTransform(const OrthoProjInfo& p);
private:
};