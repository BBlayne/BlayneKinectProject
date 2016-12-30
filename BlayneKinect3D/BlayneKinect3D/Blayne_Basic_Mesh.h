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
#include "Blayne_Utilities.h" // Includes Blayne_Types.h
#include "Blayne_Pipeline.h"

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

class BasicMesh
{
public:
	BasicMesh();
	BasicMesh(const BasicMesh* MeshToBeDeepCopied);
	~BasicMesh();

	bool CreatePrism(GLfloat length, const std::string& Filename);
	bool CreatePrism(GLfloat pointA, GLfloat pointB, const std::string& Filename);
	bool LoadMesh(const std::string& Filename);

	void Render();

	void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

	Orientation& GetOrientation() { return m_orientation; }
	Orientation GetOrientation() const { return m_orientation; }

	std::string ObjName = "";
	
	std::vector<BasicMeshEntry> getBasicMeshEntries() const { return m_Entries; };
	std::vector<Texture*> getTextures() const { return m_Textures; };

	vector<glm::vec3> m_Positions;
	vector<glm::vec3> m_Normals;
	vector<glm::vec2> m_TexCoords;
	vector<unsigned int> m_Indices;
	
private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<unsigned int>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	enum VB_TYPES {
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		WVP_MAT_VB,
		WORLD_MAT_VB,
		NUM_VBS
	};

//#define INDEX_BUFFER 0    
//#define POS_VB       1
//#define NORMAL_VB    2
//#define TEXCOORD_VB  3    
//#define WVP_MAT_VB   4
//#define WORLD_MAT_VB 5

	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBS];

	std::vector<BasicMeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	Orientation m_orientation;
};