#pragma once
#include <map>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <Kinect.h>

namespace Blayne_Types {
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = normal;
		}
	};

	struct Blayne_JointOrientations
	{
		JointType m_jointType;
		glm::quat m_orientation;
		glm::vec3 m_jointPosition;
	};

	struct BoneNameJointOrientations
	{
		std::pair<std::string, JointType> m_boneNameJoint;
		glm::quat m_orientation;
		glm::vec3 m_jointPosition;
	};
}

