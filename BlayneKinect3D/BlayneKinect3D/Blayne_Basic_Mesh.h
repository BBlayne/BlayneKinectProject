#pragma once
#include <map>
#include <vector>
#include <GL/glew.h>
#include "GL\freeglut.h"
#include "assimp\Importer.hpp" //C++ importer interface
#include "assimp\scene.h"       // Output data structure
#include "assimp\postprocess.h" // Post processing flags
#include "bTexture.h"
#include "vboindexer.hpp"


#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\matrix_decompose.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\vector_angle.hpp"

#include "Blayne_3D_Math.h"
#include "Blayne_Utilities.h"
#include "Blayne_Pipeline.h"

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;
	glm::vec3 m_normal;

	Vertex() {}

	Vertex(const glm::vec3& pos, const glm::vec3& tex, const glm::vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class BasicMesh
{
public:
	BasicMesh();

	~BasicMesh();

	bool LoadMesh(const std::string& Filename);

	void Render();

	void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

	Orientation& GetOrientation() { return m_orientation; }

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<unsigned int>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5

	GLuint m_VAO;
	GLuint m_Buffers[6];

	struct BasicMeshEntry {
		BasicMeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	std::vector<BasicMeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	Orientation m_orientation;
};