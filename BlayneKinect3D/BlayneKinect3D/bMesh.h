#pragma once
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "bTexture.h"
#include "vboindexer.hpp"

#include <map>
#include <vector>
#include "GL\freeglut.h"
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\matrix_decompose.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\vector_angle.hpp"

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define GLCheckError() (glGetError() == GL_NO_ERROR)

//typedef unsigned int glm::uint;

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

class bMesh {
public:
	bMesh();
	bMesh(double length);
	~bMesh();

	glm::mat4 armature;

	bool LoadMesh(const std::string& Filename);
	void Render();
	// recursively update the skeleton from manual animation
	//void TransformBone(const aiNode* pNode, const glm::mat4& ParentTransform);
	// calculate angle of rotation and then update tree
	void rotateBone(std::string boneName, glm::vec3 oldVec, glm::vec3 newVec);
	void rotateBoneAtFrame(std::string boneName, glm::vec3 oldPos, glm::vec3 newPos, int frame, aiScene* _scene);
	const aiScene* getScene() { return this->m_pScene; };
	//
	void BoneTransformSimplifiedNoInterpolation(std::vector<glm::mat4>& Transforms);
	void UpdateRootBone(glm::mat4 rot);
	void CreateAnimation();
	void PadImportedAnimationFrames(aiScene* _scene);
	void CopyAnimationAtFrame(int frame);

	glm::uint NumBones() const
	{
		return m_NumBones;
	}

	struct skeleton_mesh
	{
		bMesh* _BoneMesh;
		std::string _BoneName;

		skeleton_mesh() {}
		skeleton_mesh(bMesh* BoneMesh, std::string BoneName) {
			_BoneMesh = BoneMesh;
			_BoneName = BoneName;
		}
	};


	void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
	void BoneTransformAtFrame(float frame, std::vector<glm::mat4>& Transforms, aiScene* _scene);
	void PrintMatrix(glm::mat4 matrix);
	void PrintAiMatrix(aiMatrix4x4 matrix);
	void PrintVector4(glm::vec4 _vector);
	void PrintVector3(glm::vec3 _vector);
	std::string FindClosestBone(const aiMesh* pMesh, glm::vec3 _pos);
	void createSkeleton(const aiMesh* pMesh);
	const int ID;
	std::vector<skeleton_mesh> skeleton;
	glm::vec3 position;
	glm::mat4 mTransform;
	glm::mat4 rotation;
	glm::mat4 scale;
	bool createPrism(double length);
	void RenderPrism();
	bool createLine(double length);
	void RenderLine();
	void bMesh::InsertKeyFrame(glm::uint frame, const aiNode* pNode, aiScene* _scene);
	aiScene* customScene;
	
private:
	#define NUM_BONES_PER_VEREX 4
	static int currID;
	// stuff
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint VertexArrayID;



	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;

		BoneInfo()
		{
			BoneOffset = glm::mat4(1.0);
			FinalTransformation = glm::mat4(1.0);
		}

	};
	
	struct VertexBoneData
	{
		glm::uint IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];
		
		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			// set memory to zero
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBoneData(glm::uint BoneID, float Weight);
		
	};	

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	glm::uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	glm::uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	glm::uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	void FindScalingAtFrame(aiVector3D& Out, float frame, const aiNodeAnim* pNodeAnim);
	void FindRotationAtFrame(aiQuaternion& Out, float frame, const aiNodeAnim* pNodeAnim);
	void FindPositionAtFrame(aiVector3D& Out, float frame, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void ReadNodeHeirarchySimplified(const aiNode* pNode, const glm::mat4& ParentTransform);
	void ReadNodeHeirarchyAtFrame(float frame, const aiNode* pNode, const glm::mat4& ParentTransform, 
								  aiScene* _scene);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	//bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices);
	void LoadBones(glm::uint MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	aiMatrix4x4 fromMatrix3x3(const aiMatrix3x3& AssimpMatrix);
	void Clear();

	#define INVALID_MATERIAL 0xFFFFFFFF

	enum VB_TYPES {
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};

	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBs];

	struct MeshEntry {
		MeshEntry()
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

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	std::map<std::string, glm::uint> m_BoneMapping; // maps a bone name to its index
	glm::uint m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;
	glm::mat4 m_GlobalInverseTransform;

	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

	glm::vec3 FindBonePosition(const std::string NodeName);
	void FindBone(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform,
		glm::mat4& matrixToReturn);
	void FindBoneAtFrame(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform,
		glm::mat4& matrixToReturn, aiScene* _scene, int frame);
	void UpdateSkeleton(const aiMesh* pMesh);
	void UpdateSkeletonAtFrame(int frame, const aiMesh* pMesh);
	void FindLocalPosition(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::vec3& _pos);
	void FindLocalRotation(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::quat& _rot);
	void FindLocalScale(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::vec3& _scale);
	
};

