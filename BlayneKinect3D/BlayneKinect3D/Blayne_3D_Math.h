#pragma once
#include "assimp\Importer.hpp"
#include <glm\glm.hpp>

class Blayne_3D_Math {
public:
	static aiMatrix4x4 fromMatrix3x3(const aiMatrix3x3& AssimpMatrix);
	static void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to);
	static void CopyGlMatToAiMat(const glm::mat4 from, aiMatrix4x4 &to);
	static void PrintMatrix(glm::mat4 matrix);
	static void PrintAiMatrix(aiMatrix4x4 matrix);
	static void PrintVector4(glm::vec4 _vector);
	static void PrintVector3(glm::vec3 _vector);
private:
};