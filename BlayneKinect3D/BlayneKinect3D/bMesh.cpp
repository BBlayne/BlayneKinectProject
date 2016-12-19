#include "bMesh.h"
#include "Blayne_3D_Math.h"

#include <algorithm>
#include <exception>

bMesh::bMesh() : ID(currID++)
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
	m_NumBones = 0;
	m_pScene = NULL;
}

bMesh::bMesh(double length) : ID(currID++)
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
	m_NumBones = 0;
	m_pScene = NULL;

	this->createPrism(length);
}

bMesh::~bMesh()
{
	Clear();
}

void bMesh::RenderPrism()
{
	glBindVertexArray(m_VAO);

	for (glm::uint i = 0; i < m_Entries.size(); i++) {
		const glm::uint MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(glm::uint) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}

void bMesh::Clear()
{
	for (glm::uint i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}

	if (m_Buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to) {
	to[0][0] = from->a1; to[1][0] = from->a2;
	to[2][0] = from->a3; to[3][0] = from->a4;
	to[0][1] = from->b1; to[1][1] = from->b2;
	to[2][1] = from->b3; to[3][1] = from->b4;
	to[0][2] = from->c1; to[1][2] = from->c2;
	to[2][2] = from->c3; to[3][2] = from->c4;
	to[0][3] = from->d1; to[1][3] = from->d2;
	to[2][3] = from->d3; to[3][3] = from->d4;
}

void CopyGlMatToAiMat(const glm::mat4 from, aiMatrix4x4 &to) {
	to[0][0] = from[0][0]; to[0][1] = from[1][0]; to[0][2] = from[2][0]; to[0][3] = from[3][0];
	to[1][0] = from[0][1]; to[1][1] = from[1][1]; to[1][2] = from[2][1]; to[1][3] = from[3][1];
	to[2][0] = from[0][2]; to[2][1] = from[1][2]; to[2][2] = from[2][2]; to[2][3] = from[3][2];
	to[3][0] = from[0][3]; to[3][1] = from[1][3]; to[3][2] = from[2][3]; to[3][3] = from[3][3];
}

bool bMesh::createPrism(double length)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	
	GLfloat g_vertex_buffer_data[] = {
		-1.0f,-length,-1.0f,
		-1.0f,-length, 1.0f,
		-1.0f, length, 1.0f,
		1.0f, length,-1.0f,
		-1.0f,-length,-1.0f,
		-1.0f, length,-1.0f,
		1.0f,-length, 1.0f,
		-1.0f,-length,-1.0f,
		1.0f,-length,-1.0f,
		1.0f, length,-1.0f,
		1.0f,-length,-1.0f,
		-1.0f,-length,-1.0f,
		-1.0f,-length,-1.0f,
		-1.0f, length, 1.0f,
		-1.0f, length,-1.0f,
		1.0f,-length, 1.0f,
		-1.0f,-length, 1.0f,
		-1.0f,-length,-1.0f,
		-1.0f, length, 1.0f,
		-1.0f,-length, 1.0f,
		1.0f,-length, 1.0f,
		1.0f, length, 1.0f,
		1.0f,-length,-1.0f,
		1.0f, length,-1.0f,
		1.0f,-length,-1.0f,
		1.0f, length, 1.0f,
		1.0f,-length, 1.0f,
		1.0f, length, 1.0f,
		1.0f, length,-1.0f,
		-1.0f, length,-1.0f,
		1.0f, length, 1.0f,
		-1.0f, length,-1.0f,
		-1.0f, length, 1.0f,
		1.0f, length, 1.0f,
		-1.0f, length, 1.0f,
		1.0f,-length, 1.0f
	};
	
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	static const GLfloat g_normal_buffer_data[] = {
		-1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,
		0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,
		0.999998,0.000210,-0.002219,0.999998,0.000210,-0.002219,0.999998,0.000210,-0.002219,0.999998,0.000210,-0.002219,
		0.009158,-0.999916,0.009158,0.009158,-0.999916,0.009158,0.009158,-0.999916,0.009158,0.009158,-0.999916,0.009158,
		0.000000,0.000000,-1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,-1.000000,
		-0.002844,0.000270,0.999996,-0.002844,0.000270,0.999996,-0.002844,0.000270,0.999996,-0.002844,0.000270,0.999996
	};

	// Two UV coordinatesfor each vertex. They were created withe Blender.
	static const GLfloat g_uv_buffer_data[] = {
		0.000087,0.498520,
		0.001073,0.248811,
		0.999913,0.499507,
		0.500492,0.000087,
		0.750202,0.000087,
		0.750203,0.249797,
		0.500492,0.249797,
		0.999913,0.249797,
		0.750203,0.499508,
		0.750203,0.749218,
		0.500926,0.750643,
		0.500492,0.499508,
		0.249359,0.500619,
		0.250781,0.249798
	};


	std::vector<GLfloat> vertices(g_vertex_buffer_data, 
		g_vertex_buffer_data + sizeof(g_vertex_buffer_data) / sizeof(*g_vertex_buffer_data));
	std::vector<GLfloat> colours(g_color_buffer_data,
		g_color_buffer_data + sizeof(g_color_buffer_data) / sizeof(*g_color_buffer_data));
	std::vector<GLfloat> normal_points(g_normal_buffer_data,
		g_normal_buffer_data + sizeof(g_normal_buffer_data) / sizeof(*g_normal_buffer_data));

	std::vector<GLfloat> uv_points(g_uv_buffer_data,
		g_uv_buffer_data + sizeof(g_uv_buffer_data) / sizeof(*g_uv_buffer_data));


	std::vector<glm::vec3> Positions;
	for (int i = 0; i < vertices.size(); i += 3)
	{
		glm::vec3 _pos(vertices[i], vertices[i + 1], vertices[i + 2]);
		Positions.push_back(_pos);
	}

	std::vector<glm::vec2> TexCoords;
	for (int i = 0; i < uv_points.size(); i += 2)
	{
		glm::vec2 _uv(uv_points[i], uv_points[i + 1]);
		TexCoords.push_back(_uv);
	}

	std::vector<glm::vec3> Normals;
	for (int i = 0; i < normal_points.size(); i += 3)
	{
		glm::vec3 _pos(normal_points[i], normal_points[i + 1], normal_points[i + 2]);
		Normals.push_back(_pos);
	}

	std::vector<glm::uint> Indices;
	MeshEntry entry;	
	m_Entries.push_back(entry);
	Texture* tex = new Texture(GL_TEXTURE_2D, std::string("grid.png").c_str());
	m_Textures.push_back(tex);

	if (!m_Textures[0]->Load()) {
		printf("Error loading texture '%s'\n", std::string("grid.png").c_str());
		delete m_Textures[0];
		m_Textures[0] = NULL;
		return false;
	}
	else {
		printf("%d - loaded texture '%s'\n", 0, std::string("grid.png").c_str());
	}

	// Populate the index buffer
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(Positions, TexCoords, Normals, Indices, indexed_vertices, indexed_uvs,
		indexed_normals);

	printf("NumFaces %d, numVerts %d, numNormals %d, numIndices %d \n", Positions.size() / 3,
		Positions.size(), Normals.size(), Indices.size());
	//std::cin.get();

	glm::uint NumVertices = 0;
	glm::uint NumIndices = 0;
	m_Entries[0].MaterialIndex = 0;
	m_Entries[0].NumIndices = Indices.size();
	m_Entries[0].BaseVertex = NumVertices;
	m_Entries[0].BaseIndex = NumIndices;
	NumVertices += Positions.size();
	NumIndices += Indices.size();

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);



	return GLCheckError();
}



bool bMesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool Ret = false;

	m_pScene = (aiScene*)m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (m_pScene) {
		CopyaiMat(&m_pScene->mRootNode->mTransformation, m_GlobalInverseTransform);
		//m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
		//m_GlobalInverseTransform.Inverse();
		Ret = InitFromScene(m_pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}

bool bMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<glm::uint> Indices;

	glm::uint NumVertices = 0;
	glm::uint NumIndices = 0;

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;
		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (glm::uint i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];		
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}
	
	if (!InitMaterials(pScene, Filename)) {
		fprintf(stderr, "Failed to load Materials\n");
		std::cin.get();
		return false;
	}

	printf("NumFaces %d, numVerts %d, numNormals %d, numIndices %d \n", Positions.size() / 3,
		Positions.size(), Normals.size(), Indices.size());

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		cerr << "OpenGL error: " << err << endl;
		string error;
		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		cerr << "GL_" << error.c_str() << " - " << endl;
	}

	return GLCheckError();
}

void bMesh::InitMesh(unsigned int MeshIndex,
	const aiMesh* paiMesh,
	std::vector<glm::vec3>& Positions,
	std::vector<glm::vec3>& Normals,
	std::vector<glm::vec2>& TexCoords,
	std::vector<VertexBoneData>& Bones,
	std::vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	LoadBones(MeshIndex, paiMesh, Bones);
	printf("InitMesh: Num Bones: %d\n", m_NumBones);
	printf("InitMesh: NumBoneInfo: %d\n", m_BoneInfo.size());

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

bool bMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (glm::uint i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}

void bMesh::Render()
{
	glBindVertexArray(m_VAO);

	for (glm::uint i = 0; i < m_Entries.size(); i++) {
		const glm::uint MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(glm::uint) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}

void bMesh::LoadBones(glm::uint MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	for (glm::uint i = 0; i < pMesh->mNumBones; i++) {
		glm::uint BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);
		printf("Bone: %s \n", BoneName.c_str());

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;			
			if (BoneName == "Hips")
			{
				//printf("Hips. #%d\n", BoneIndex);
			}
			m_BoneInfo.push_back(bi);			
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		m_BoneMapping[BoneName] = BoneIndex;
		CopyaiMat(&pMesh->mBones[i]->mOffsetMatrix, m_BoneInfo[BoneIndex].BoneOffset);

		//PrintMatrix(m_BoneInfo[BoneIndex].BoneOffset);

		for (glm::uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			glm::uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void bMesh::PrintMatrix(glm::mat4 matrix)
{
	printf("Printing matrix: \n");
	printf("%f, %f, %f, %f \n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]);
	printf("%f, %f, %f, %f \n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]);
	printf("%f, %f, %f, %f \n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]);
	printf("%f, %f, %f, %f \n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
}

void bMesh::PrintAiMatrix(aiMatrix4x4 matrix)
{
	printf("Printing aiMatrix: \n");
	printf("%f, %f, %f, %f \n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]);
	printf("%f, %f, %f, %f \n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]);
	printf("%f, %f, %f, %f \n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]);
	printf("%f, %f, %f, %f \n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
}

void bMesh::PrintVector4(glm::vec4 _vector)
{
	printf("Printing Vector: \n");
	printf("%f, %f, %f, %f \n", _vector.x, _vector.y, _vector.z, _vector.w);
}

void bMesh::PrintVector3(glm::vec3 _vector)
{
	printf("Printing Vector: \n");
	printf("%f, %f, %f \n", _vector.x, _vector.y, _vector.z);
}

void bMesh::VertexBoneData::AddBoneData(glm::uint BoneID, float Weight)
{
	for (glm::uint i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}


void bMesh::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	if (this->m_pScene->HasAnimations())
	{
		float TicksPerSecond = m_pScene->mAnimations[0]->mTicksPerSecond != 0 ?
			m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f;
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, m_pScene->mAnimations[0]->mDuration);
		//printf("BoneTransform: AnimTime. %f \n", AnimationTime);
		/*
		printf("TimeSeconds: %f: AnimationTime: %f: TicksPS: %f: TinTicks: %f, mDur: %f \n", 
			TimeInSeconds, 
			AnimationTime, TicksPerSecond, TimeInTicks,
			m_pScene->mAnimations[0]->mDuration);
			*/
		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);
		Transforms.clear();
		Transforms.resize(m_NumBones);
		//printf("BoneTransform, numBones: %d \n", m_NumBones);

		for (glm::uint i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}
	else
	{
		printf("Error, no animations present.");
	}
}

void bMesh::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms, int _animation)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	if (this->m_pScene->HasAnimations())
	{
		float TicksPerSecond = m_pScene->mAnimations[_animation]->mTicksPerSecond != 0 ?
			m_pScene->mAnimations[_animation]->mTicksPerSecond : 25.0f;
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, m_pScene->mAnimations[_animation]->mDuration);
		//printf("BoneTransform: AnimTime. %f \n", AnimationTime);
		printf("TimeSeconds: %f: AnimationTime: %f: TicksPS: %f: TinTicks: %f, mDur: %f \n",
			TimeInSeconds,
			AnimationTime, TicksPerSecond, TimeInTicks,
			m_pScene->mAnimations[_animation]->mDuration);
		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity, _animation);
		Transforms.clear();
		Transforms.resize(m_NumBones);
		//printf("BoneTransform, numBones: %d \n", m_NumBones);

		for (glm::uint i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}
	else
	{
		printf("Error, no animations present.");
	}
}

void bMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = m_pScene->mAnimations[0];
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		glm::vec3 mScalingVector(Scaling.x, Scaling.y, Scaling.z);
		ScalingM = glm::scale(mScalingVector);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;		
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM;
		CopyaiMat(&fromMatrix3x3(RotationQ.GetMatrix()), RotationM);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(glm::vec3(Translation.x, Translation.y, Translation.z));
		std::string BoneName(pNode->mName.data);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation;

	GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void bMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, int _anim)
{
	std::string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = m_pScene->mAnimations[_anim];
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		glm::vec3 mScalingVector(Scaling.x, Scaling.y, Scaling.z);
		ScalingM = glm::scale(mScalingVector);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM;
		CopyaiMat(&fromMatrix3x3(RotationQ.GetMatrix()), RotationM);
		//glm::quat myQuat = glm::quat(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		//RotationM = glm::toMat4(myQuat);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(glm::vec3(Translation.x, Translation.y, Translation.z));
		std::string BoneName(pNode->mName.data);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation;

	GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, _anim);
	}
}

aiMatrix4x4 bMesh::fromMatrix3x3(const aiMatrix3x3& AssimpMatrix)
{
	aiMatrix4x4 m(AssimpMatrix);
	return m;
}

void bMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	glm::uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	glm::uint NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	//
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

glm::uint bMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime <= (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

glm::uint bMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime <= (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
	assert(0);
}

glm::uint bMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (glm::uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime <= (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}

void bMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	glm::uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	glm::uint NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void bMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	glm::uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	glm::uint NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	if (std::string(pNodeAnim->mNodeName.C_Str()) == "UpperArm.L")
		printf("NodeName: %s; NextPosition(%u): %.3f, Position(%u): %.3f \n", std::string(pNodeAnim->mNodeName.C_Str()), NextPositionIndex,
		pNodeAnim->mPositionKeys[NextPositionIndex].mTime, PositionIndex, pNodeAnim->mPositionKeys[PositionIndex].mTime);

	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim* bMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (glm::uint i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

glm::vec3 bMesh::FindBonePosition(const std::string NodeName)
{
	
	glm::mat4 myMatrix = glm::mat4(1.0);
	FindBone(NodeName, this->m_pScene->mRootNode, glm::mat4(1.0), myMatrix);
	glm::vec3 myPos;
	myPos.x = myMatrix[3][0];
	myPos.y = myMatrix[3][1];
	myPos.z = 0;

	return myPos;
}

void bMesh::FindBone(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, 
	glm::mat4& matrixToReturn)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);

	if (NodeName.compare(BoneToFind) == 0)
	{
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		matrixToReturn = ParentTransform * NodeTransformation;
		
	}
	else
	{
		for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
			FindBone(BoneToFind, pNode->mChildren[i], ParentTransform * NodeTransformation, matrixToReturn);
		}
	}
}

float getDistanceToBone(glm::vec3 _from, glm::vec3 _to)
{
	return glm::distance(_from, _to);
}

glm::vec3 FindMidPoint(glm::vec3 ptA, glm::vec3 ptB)
{
	glm::vec3 mid = glm::vec3((ptA.x + ptB.x) / 2, (ptA.y + ptB.y) / 2, (ptA.z + ptB.z) / 2);
	float length = sqrt(((ptA.x - ptB.x) * (ptA.x - ptB.x)) +
		((ptA.y - ptB.y) * (ptA.y - ptB.y)) +
		((ptA.z - ptB.z) * (ptA.z - ptB.z)));

	mid = glm::vec3(mid.x, mid.y, mid.z);
	return mid;
}

void bMesh::createSkeleton(const aiMesh* pMesh)
{
	printf("Creating Skeleton \n");
	for (glm::uint i = 0; i < pMesh->mNumBones; i++)
	{
		std::string BoneName(pMesh->mBones[i]->mName.data);
		// Leaf node?
		int numChildren = this->getScene()->mRootNode->FindNode(BoneName.c_str())->mNumChildren;
		//printf("numChildren of %s is %d \n", BoneName, numChildren);
		if (BoneName.compare("Root") == 0 || BoneName.compare("Armature") == 0 || numChildren == 0)
		{
			continue;
		}

		// Temporary Head Position
		//_bone->position = FindBonePosition(BoneName);
		glm::vec3 head = FindBonePosition(BoneName);
		// Temporary tails
		// Reasonably sure each bone we iterate over has exactly one child.
		// But would probably work if there were many children.
		std::string TailBoneName(this->getScene()->mRootNode->FindNode(BoneName.c_str())->mChildren[0]->mName.data);
		glm::vec3 tail = FindBonePosition(TailBoneName);
		float length = glm::distance(tail, head);
		printf("Length of %s to %s is %.3f\n", BoneName, TailBoneName, length);
		bMesh* _bone = new bMesh(length / 2);


		_bone->position = FindMidPoint(head, tail);
		glm::mat4 mMatrix = glm::mat4(1.0);
		FindBone(BoneName, this->m_pScene->mRootNode, glm::mat4(1.0), mMatrix);
		_bone->mTransform = mMatrix;

		glm::vec3 scale = glm::vec3(1.0);
		glm::quat rot;
		glm::vec3 trans;
		glm::vec4 per;
		glm::uint BoneIndex = m_BoneMapping[BoneName];
		glm::decompose(mMatrix, scale, rot, trans, trans, per);
		_bone->rotation = glm::mat4_cast(glm::conjugate(rot));
		_bone->scale = glm::scale(scale);
		skeleton_mesh skeleton_pair(_bone, BoneName);
		skeleton.push_back(skeleton_pair);
	}

}

void bMesh::rotateBoneAtFrame(std::string boneName, glm::quat _newRot, int frame, aiScene* _scene)
{
	glm::mat4 LocalNodeTransform;
	glm::mat4 GlobalNodeTransform;
	glm::mat4 AnimTransform;
	const aiNodeAnim* _node = FindNodeAnim(_scene->mAnimations[0], boneName);
	CopyaiMat(&_scene->mRootNode->FindNode(boneName.c_str())->mTransformation, LocalNodeTransform);

	FindBone(boneName, _scene->mRootNode, glm::mat4(1.0), GlobalNodeTransform);
	glm::quat _rotation;	

	_rotation = _newRot;
	_node->mRotationKeys[frame].mValue = aiQuaternion(_rotation.w, _rotation.x, _rotation.y, _rotation.z);
}

void bMesh::rotateBoneAtFrame(std::string boneName, glm::vec3 oldPos, glm::vec3 newPos, int frame, aiScene* _scene)
{
	// Origin to rotate from
	glm::vec3 head = FindBonePosition(boneName);
	glm::vec3 A = oldPos - head;
	glm::vec3 B = newPos - head;

	float angle = glm::angle(glm::normalize(A), glm::normalize(B));
	glm::vec3 cross = glm::cross(A, B);
	if (glm::dot(glm::vec3(0, 0, 1), cross) < 0)
	{
		angle = -angle;
	}

	glm::mat4 LocalNodeTransform;
	glm::mat4 GlobalNodeTransform;
	glm::mat4 AnimTransform;
	const aiNodeAnim* _node = FindNodeAnim(_scene->mAnimations[0], boneName);
	CopyaiMat(&fromMatrix3x3(_node->mRotationKeys[frame].mValue.GetMatrix()), AnimTransform);



	//printf("Previous Quat, frame(%d): x: %.3f, y: %.3f z: %.3f w: %.3f \n", frame,
	//	_node->mRotationKeys[frame].mValue.x, _node->mRotationKeys[frame].mValue.y,
	//	_node->mRotationKeys[frame].mValue.z, _node->mRotationKeys[frame].mValue.w);

	CopyaiMat(&_scene->mRootNode->FindNode(boneName.c_str())->mTransformation, LocalNodeTransform);
	
	FindBone(boneName, _scene->mRootNode, glm::mat4(1.0), GlobalNodeTransform);

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 1), // Camera in World Space
		glm::vec3(0, 0, 0), // and looks at 
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::quat globalRot;
	glm::decompose(GlobalNodeTransform, glm::vec3(1.0), globalRot, glm::vec3(1.0), glm::vec3(1.0), glm::vec4(1.0));

	glm::mat4 globalRotMat = glm::mat4_cast(glm::conjugate(globalRot));

	glm::mat4 BoneToView = globalRotMat * View;
	glm::mat4 BoneToViewInverse = glm::inverse(BoneToView);
	glm::vec4 BoneSpaceRotationAxis = BoneToViewInverse * glm::vec4(0, 0, 1, 0);
	glm::mat4 newRotation = glm::rotate(angle, glm::normalize(glm::vec3(BoneSpaceRotationAxis)));


	AnimTransform = AnimTransform * newRotation;



	glm::quat _rotation = glm::toQuat(AnimTransform);
	//_rotation = glm::quat(0, 0, 0, 0);
	_node->mRotationKeys[frame].mValue = aiQuaternion(_rotation.w, _rotation.x, _rotation.y, _rotation.z);
}

void bMesh::FindBoneAtFrame(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform,
	glm::mat4& matrixToReturn, aiScene* _scene, int frame)
{

}

void bMesh::rotateBone(std::string boneName, glm::vec3 oldPos, glm::vec3 newPos)
{
	//glm::mat4 NodeTransformation = glm::mat4(1.0);
	//NodeTransformation = glm::rotate(90.0f, glm::vec3(0, 0, 1));

	//this->getScene()->mRootNode->FindNode(std::string("UpperArm.R").c_str())->mTransformation;
	// Origin to rotate from
	glm::vec3 head = FindBonePosition(boneName); 
	glm::vec3 A = oldPos - head;
	glm::vec3 B = newPos - head;

	float angle = glm::angle(glm::normalize(A), glm::normalize(B));
	glm::vec3 cross = glm::cross(A, B);
	if (glm::dot(glm::vec3(0, 0, 1), cross) < 0)
	{
		angle = -angle;
	}
	
	glm::mat4 LocalNodeTransform;
	glm::mat4 GlobalNodeTransform;
	CopyaiMat(&this->getScene()->mRootNode->FindNode(boneName.c_str())->mTransformation, LocalNodeTransform);
	FindBone(boneName, this->getScene()->mRootNode, glm::mat4(1.0), GlobalNodeTransform);
	//glm::mat4 newRotation = glm::rotate(angle, glm::vec3(axis.x, axis.y, axis.z));
	

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 1), // Camera in World Space
		glm::vec3(0, 0, 0), // and looks at 
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// NEED GLOBAL ROTATION
	//CurrentNodeTransform = newRotation * CurrentNodeTransform;
	//CurrentNodeTransform = (CurrentNodeTransform * newRotation);

	glm::mat4 BoneToView = GlobalNodeTransform * View;
	glm::mat4 BoneToViewInverse = glm::inverse(BoneToView);
	glm::vec4 BoneSpaceRotationAxis = BoneToViewInverse * glm::vec4(0, 0, 1, 0);
	//printf("Printing BoneSpaceRotationAxis \n");
	//PrintVector3(glm::normalize(BoneSpaceRotationAxis));
	glm::mat4 newRotation = glm::rotate(angle, glm::normalize(glm::vec3(BoneSpaceRotationAxis)));

	LocalNodeTransform = LocalNodeTransform * newRotation;

	CopyGlMatToAiMat(LocalNodeTransform, this->getScene()->mRootNode->FindNode(boneName.c_str())->mTransformation);

}

// Find the closest bone to the given point.
std::string bMesh::FindClosestBone(const aiMesh* pMesh, glm::vec3 _pos)
{

	glm::vec3 pos;
	float shortestDistance = 99999; // some large number
	std::string closestBone;
	glm::vec3 shortestPos;
	//printf("Clicked: %.3f, %.3f, %.3f \n", _pos.x, _pos.y, _pos.z);
	for (glm::uint i = 0; i < pMesh->mNumBones; i++) {
		glm::uint BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);
		int numChildren = this->getScene()->mRootNode->FindNode(BoneName.c_str())->mNumChildren;
		if (BoneName.compare("Root") == 0 || BoneName.compare("Armature") == 0 || numChildren == 0)
		{
			continue;
		}
		
		glm::vec3 head = FindBonePosition(BoneName);
		std::string TailBoneName(this->getScene()->mRootNode->FindNode(BoneName.c_str())->mChildren[0]->mName.data);
		glm::vec3 tail = FindBonePosition(TailBoneName);
		pos = FindMidPoint(head, tail);
		float new_possibly_shortest_dist = getDistanceToBone(_pos, pos);		
		//printf("Possibly Shortest Dist (%s): %.3f at (%.3f, %.3f, %.3f) \n", BoneName, new_possibly_shortest_dist, 
		//	pos.x, pos.y, pos.z);
		if (shortestDistance > new_possibly_shortest_dist)
		{
			shortestDistance = new_possibly_shortest_dist;
			closestBone = BoneName;
			shortestPos = pos;
		}

		//printf("Current Shortest is (%s) at %.3f \n", closestBone, shortestDistance);
	}

	//printf("Closest bone is %s located at %.3f, %.3f, %.3f:  \n", closestBone, 
	//	shortestPos.x, shortestPos.y, shortestPos.z);

	return closestBone;

}

void bMesh::UpdateRootBone(glm::mat4 rot)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	//m_pScene->mRootNode
	std::string NodeName(m_pScene->mRootNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&m_pScene->mRootNode->mTransformation, NodeTransformation);
	NodeTransformation = rot * NodeTransformation;
	CopyGlMatToAiMat(NodeTransformation, this->getScene()->mRootNode->mTransformation);
}

int bMesh::currID = 0;

void bMesh::BoneTransformSimplifiedNoInterpolation(std::vector<glm::mat4>& Transforms)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	ReadNodeHeirarchySimplified(m_pScene->mRootNode, Identity);
	Transforms.clear();
	Transforms.resize(m_NumBones);

	for (glm::uint i = 0; i < m_NumBones; i++) {
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}

	UpdateSkeleton(this->getScene()->mMeshes[0]);
}

void bMesh::ReadNodeHeirarchySimplified(const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	std::string BoneName(pNode->mName.data);

	glm::mat4 GlobalTransformation;
	GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchySimplified(pNode->mChildren[i], GlobalTransformation);
	}
}

void bMesh::UpdateSkeleton(const aiMesh* pMesh)
{

	//printf("Updating Skeleton \n");
	for (glm::uint i = 0; i < pMesh->mNumBones; i++)
	{
		std::string BoneName(pMesh->mBones[i]->mName.data);
		// Leaf node?
		int numChildren = this->getScene()->mRootNode->FindNode(BoneName.c_str())->mNumChildren;
		//printf("numChildren of %s is %d \n", BoneName, numChildren);
		if (BoneName.compare("Root") == 0 || BoneName.compare("Armature") == 0 || numChildren == 0)
		{
			continue;
		}

		// Temporary Head
		glm::vec3 head = FindBonePosition(BoneName);
		// Temporary tail
		std::string TailBoneName(this->getScene()->mRootNode->FindNode(BoneName.c_str())->mChildren[0]->mName.data);
		glm::vec3 tail = FindBonePosition(TailBoneName);
		//_bone->position = FindMidPoint(head, tail);

		glm::mat4 NodeTransform = glm::mat4(1.0);
		for each (skeleton_mesh var in skeleton)
		{
			if (var._BoneName.compare(BoneName) == 0)
			{
				FindBone(var._BoneName, this->getScene()->mRootNode,
					glm::mat4(1.0), NodeTransform);
				glm::vec3 scale = glm::vec3(1.0);
				glm::quat rot;
				glm::vec3 trans;
				glm::vec4 per;
				glm::decompose(NodeTransform, scale, rot, trans, trans, per);
				var._BoneMesh->rotation = glm::mat4_cast(glm::conjugate(rot));
				var._BoneMesh->scale = glm::scale(scale);
				var._BoneMesh->mTransform = NodeTransform;
				var._BoneMesh->position = FindMidPoint(head, tail);
			}
		}
	}
}

void bMesh::InsertKeyFrame(glm::uint frame, int _animation, const aiNode* p_rootNode, aiScene* _scene)
{
	// Get the duration of the animation
	int numFrames = _scene->mAnimations[_animation]->mDuration;
	// frame is the "time"-frame we are inserting a new keyframe.
	// Either we are placing an existing keyframe, namely,
	// mTime == frame
	// Or we are inserting an entirely new frame, mTime != frame

	printf("Inserting Key Frame to %s \n", _scene->mAnimations[_animation]->mName.C_Str());

	// If the inserted frame is at time > the duration, increase duration
	// Should be gauranteed.
	if (frame > numFrames || frame == 0)
		_scene->mAnimations[_animation]->mDuration++;

	// Iterate over each animation node
	for (glm::uint i = 0; i < _scene->mAnimations[_animation]->mNumChannels; i++)
	{
		
		std::string BoneName(_scene->mAnimations[_animation]->mChannels[i]->mNodeName.data);
		// Skip past our non-deform bones
		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			printf("Skipping %s\n", BoneName);
			continue;
		}

		aiNodeAnim* _AnimBone = _scene->mAnimations[_animation]->mChannels[i];

		// Position Keys
		int m_keyframe = -1;
		for (glm::uint j = 0; j < _AnimBone->mNumPositionKeys; j++) {
			if (frame == (int)_AnimBone->mPositionKeys[j].mTime) {
				// We found a keyframe that matches, we can replace it.
				m_keyframe = j;
				break;
			}
		}

		if (m_keyframe == -1)
		{
			// Creating new entry.
			// Copy mPositionKeys to Vector for easy insertion
			std::vector<aiVectorKey> m_PositionKeys(_AnimBone->mPositionKeys, 
				_AnimBone->mPositionKeys + _AnimBone->mNumPositionKeys);

			aiVectorKey m_PositionKey;
			m_PositionKey.mTime = frame;
			m_PositionKey.mValue = aiVector3D();
			// Insert the new entry
			m_PositionKeys.insert(std::upper_bound(m_PositionKeys.begin(), m_PositionKeys.end(), m_PositionKey), m_PositionKey);
			delete(_scene->mAnimations[_animation]->mChannels[i]->mPositionKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mPositionKeys = new aiVectorKey[(_AnimBone->mNumPositionKeys + 1) * 2];
			std::copy(m_PositionKeys.begin(), m_PositionKeys.end(), _scene->mAnimations[_animation]->mChannels[i]->mPositionKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mNumPositionKeys++;
		}
		// Scale Keys
		m_keyframe = -1;
		for (glm::uint j = 0; j < _AnimBone->mNumScalingKeys; j++) {
			if (frame == (int)_AnimBone->mScalingKeys[j].mTime) {
				// We found a keyframe that matches, we can replace it.
				m_keyframe = j;
				break;
			}
		}

		if (m_keyframe == -1)
		{
			// Creating new entry.
			// Copy mPositionKeys to Vector for easy insertion
			std::vector<aiVectorKey> m_ScalingKeys(_AnimBone->mScalingKeys,
				_AnimBone->mScalingKeys + _AnimBone->mNumScalingKeys);

			aiVectorKey m_ScalingKey;
			m_ScalingKey.mTime = frame;
			m_ScalingKey.mValue = aiVector3D();
			// Insert the new entry
			m_ScalingKeys.insert(std::upper_bound(m_ScalingKeys.begin(), m_ScalingKeys.end(), m_ScalingKey), m_ScalingKey);
			delete(_scene->mAnimations[_animation]->mChannels[i]->mScalingKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mScalingKeys = new aiVectorKey[(_AnimBone->mNumScalingKeys + 1) * 2];
			std::copy(m_ScalingKeys.begin(), m_ScalingKeys.end(), _scene->mAnimations[_animation]->mChannels[i]->mScalingKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mNumScalingKeys++;
		}
		// Rotation Keys
		m_keyframe = -1;
		for (glm::uint j = 0; j < _AnimBone->mNumRotationKeys; j++) {
			if (frame == (int)_AnimBone->mRotationKeys[j].mTime) {
				// We found a keyframe that matches, we can replace it.
				m_keyframe = j;
				break;
			}
		}

		if (m_keyframe == -1)
		{
			// Creating new entry.
			// Copy mPositionKeys to Vector for easy insertion
			std::vector<aiQuatKey> m_RotationKeys(_AnimBone->mRotationKeys,
				_AnimBone->mRotationKeys + _AnimBone->mNumRotationKeys);

			aiQuatKey m_RotationKey;
			m_RotationKey.mTime = frame;
			m_RotationKey.mValue = aiQuaternion();
			// Insert the new entry
			m_RotationKeys.insert(std::upper_bound(m_RotationKeys.begin(), m_RotationKeys.end(), m_RotationKey), m_RotationKey);
			delete(_scene->mAnimations[_animation]->mChannels[i]->mRotationKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mRotationKeys = new aiQuatKey[(_AnimBone->mNumRotationKeys + 1) * 2];
			std::copy(m_RotationKeys.begin(), m_RotationKeys.end(), _scene->mAnimations[_animation]->mChannels[i]->mRotationKeys);
			_scene->mAnimations[_animation]->mChannels[i]->mNumRotationKeys++;
		}

		// Since we're creating a new entry, increase duration by 1.		
	}
}

// Create a new blank animation with an array of each node anim for each bone
// Each node animation should have 0 keys of translation, rotation, and scaling.
// Key Frames will be created by the user.
// when I'm more confident, append scene to existing scene instead of making a new one.
void bMesh::CreateAnimation()
{
	// Create a new aiScene in memory
	int numAnimations = 0;

	if (this->m_pScene->HasAnimations())
	{
		numAnimations = this->m_pScene->mNumAnimations;
	}

	// incrementing the number of animations
	const int NEW_MAX_ANIMATION_COUNT = numAnimations + 1;
	
	aiAnimation** newAnimations = new aiAnimation*[NEW_MAX_ANIMATION_COUNT];
	// Create our initial blank animation at rest pose for 1 frame
	aiAnimation* mNewAnimation = new aiAnimation();

	// populate our array
	for (int i = 0; i < this->m_pScene->mNumAnimations; i++)
	{
		newAnimations[i] = this->m_pScene->mAnimations[i];
	}
	
	// Name it something generic
	mNewAnimation->mName = std::string("new Animation").append(std::to_string(NEW_MAX_ANIMATION_COUNT));
	// Allocate memory for array of node animations
	// Each bone/node has pos/rot/sca keys etc
	aiNodeAnim** mAnims = new aiNodeAnim*[this->m_pScene->mMeshes[0]->mNumBones];

	for (glm::uint i = 0; i < this->m_pScene->mMeshes[0]->mNumBones; i++)
	{
		printf("Value of MAX: %d \n", NEW_MAX_ANIMATION_COUNT);
		std::string BoneName(this->m_pScene->mMeshes[0]->mBones[i]->mName.data);
		mAnims[i] = new aiNodeAnim();
		mAnims[i]->mNodeName = BoneName;
		mAnims[i]->mNumPositionKeys = 0;
		mAnims[i]->mNumRotationKeys = 0;
		mAnims[i]->mNumScalingKeys = 0;		
	}

	mNewAnimation->mChannels = new aiNodeAnim*[this->m_pScene->mMeshes[0]->mNumBones];
	mNewAnimation->mChannels = mAnims;
	mNewAnimation->mNumChannels = this->m_pScene->mMeshes[0]->mNumBones;
	mNewAnimation->mDuration = 0;

	printf("Created new animation %s \n", std::string(mNewAnimation->mName.data));
	for (int i = 0; i < mNewAnimation->mNumChannels; i++)
	{
		printf("Created new animation node for bone: %s \n", std::string(mNewAnimation->mChannels[i]->mNodeName.data));
	}

	
	newAnimations[NEW_MAX_ANIMATION_COUNT - 1] = mNewAnimation;
	this->m_pScene->mAnimations = newAnimations;
	this->m_pScene->mNumAnimations = NEW_MAX_ANIMATION_COUNT;
	// Insert rest/default pose as first key frame
	InsertKeyFrame(0, numAnimations, this->m_pScene->mRootNode, this->m_pScene);		
	printf("Done... \n");
}

void bMesh::FindLocalPosition(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::vec3& _pos)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);

	glm::mat4 GlobalTransformation;
	GlobalTransformation = ParentTransform * NodeTransformation;

	if (NodeName.compare(BoneToFind) == 0)
	{
		glm::vec3 scale = glm::vec3(1.0);
		glm::quat rot;
		glm::vec3 trans;
		glm::vec4 per;
		glm::decompose(NodeTransformation, scale, rot, trans, trans, per);
		_pos = glm::vec3(NodeTransformation[3][0], NodeTransformation[3][1], NodeTransformation[3][2]);
	}
	else
	{
		for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
			FindLocalPosition(BoneToFind, pNode->mChildren[i], GlobalTransformation, _pos);
		}
	}
}

void bMesh::FindLocalRotation(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::quat& _rot)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);

	glm::mat4 GlobalTransformation;
	GlobalTransformation = ParentTransform * NodeTransformation;

	if (NodeName.compare(BoneToFind) == 0)
	{
		glm::vec3 scale = glm::vec3(1.0);
		glm::quat rot;
		glm::vec3 trans;
		glm::vec4 per;
		glm::decompose(NodeTransformation, scale, rot, trans, trans, per);
		_rot = rot;
	}
	else
	{
		for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
			FindLocalRotation(BoneToFind, pNode->mChildren[i], GlobalTransformation, _rot);
		}
	}
}

void bMesh::FindLocalScale(const std::string BoneToFind, const aiNode* pNode, const glm::mat4& ParentTransform, glm::vec3& _scale)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);

	glm::mat4 GlobalTransformation;
	GlobalTransformation = ParentTransform * NodeTransformation;

	if (NodeName.compare(BoneToFind) == 0)
	{
		glm::vec3 scale = glm::vec3(1.0);
		glm::quat rot;
		glm::vec3 trans;
		glm::vec4 per;
		glm::decompose(NodeTransformation, scale, rot, trans, trans, per);
		_scale = scale;
	}
	else
	{
		for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
			FindLocalScale(BoneToFind, pNode->mChildren[i], GlobalTransformation, _scale);
		}
	}
}

void bMesh::BoneTransformAtFrame(float frame, std::vector<glm::mat4>& Transforms, aiScene* _scene,
	int _anim)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	if (_scene->HasAnimations())
	{
		ReadNodeHeirarchyAtFrame(frame, _scene->mRootNode, Identity, _scene, _anim);
		Transforms.clear();
		Transforms.resize(m_NumBones);

		for (glm::uint i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}
	else
	{
		printf("Error, no animations present.");
	}
}

void bMesh::ReadNodeHeirarchyAtFrame(float frame, const aiNode* pNode, 
	const glm::mat4& ParentTransform, aiScene* _scene, int _anim)
{
	std::string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = _scene->mAnimations[_anim];
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		FindScalingAtFrame(Scaling, frame, pNodeAnim);
		glm::mat4 ScalingM;
		glm::vec3 mScalingVector(Scaling.x, Scaling.y, Scaling.z);
		ScalingM = glm::scale(mScalingVector);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		FindRotationAtFrame(RotationQ, frame, pNodeAnim);
		glm::mat4 RotationM;
		CopyaiMat(&fromMatrix3x3(RotationQ.GetMatrix()), RotationM);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		FindPositionAtFrame(Translation, frame, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(glm::vec3(Translation.x, Translation.y, Translation.z));
		std::string BoneName(pNode->mName.data);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;

	}

	glm::mat4 GlobalTransformation;

	GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchyAtFrame(frame, pNode->mChildren[i], GlobalTransformation, _scene, _anim);
	}
}

void bMesh::KinectBoneTransformAtFrame(int frame, std::vector<glm::mat4>& Transforms, aiScene* _scene, 
	int _animationToInsertInto)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	InsertKeyFrame(frame, _animationToInsertInto, _scene->mRootNode, _scene);

	if (_scene->HasAnimations())
	{
		KinectReadNodeHeirarchyAtFrame(frame, _scene->mRootNode, Identity, _scene, _animationToInsertInto);
		Transforms.clear();
		Transforms.resize(m_NumBones);

		for (glm::uint i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}
	else
	{
		printf("Error, no animations present.");
	}
}

void bMesh::KinectReadNodeHeirarchyAtFrame(int frame, const aiNode* pNode,
	const glm::mat4& ParentTransform, aiScene* _scene, int _animationToInsertInto)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	std::string BoneName(pNode->mName.data);
	const aiNodeAnim* _BoneAnimNode = FindNodeAnim(_scene->mAnimations[_animationToInsertInto], BoneName);
	glm::mat4 GlobalTransformation;

	std::vector<std::string>::iterator it;
	it = std::find(m_mask.begin(), m_mask.end(), NodeName);
	if (it != m_mask.end())
	{	
		glm::mat4 GlobalKinectRotation = glm::mat4(1.0);
		glm::quat _rotation = m_JointNameOrientations[NodeName].m_orientation;
		glm::quat parentRotation;
		glm::vec3 _pos;
		GlobalKinectRotation = glm::toMat4(_rotation);
		glm::decompose(NodeTransformation, glm::vec3(1.0), glm::quat(), _pos, glm::vec3(1.0), glm::vec4(1.0));
		glm::decompose(ParentTransform, glm::vec3(1.0), parentRotation, glm::vec3(1.0), glm::vec3(1.0), glm::vec4(1.0));
		glm::mat4 LocalRotation = glm::toMat4(glm::inverse(glm::conjugate(parentRotation))) * GlobalKinectRotation;
		NodeTransformation = glm::translate(_pos) * LocalRotation;
		GlobalTransformation = ParentTransform * NodeTransformation;
		
	}
	else
	{
		GlobalTransformation = ParentTransform * NodeTransformation;
	}

	
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;

		// Insert new keyframe
		glm::quat globalRot;
		glm::vec3 globalPos;
		glm::vec3 globalScale;
		glm::decompose(NodeTransformation, globalScale, globalRot, globalPos, glm::vec3(1.0), glm::vec4(1.0));
		globalRot = glm::conjugate(globalRot);
		int closectRotKeyFrame = 0; //FindRotation(frame, _BoneAnimNode);
		std::vector<aiVectorKey> vecKeys(_BoneAnimNode->mScalingKeys, _BoneAnimNode->mScalingKeys + _BoneAnimNode->mNumRotationKeys);
		for (glm::uint j = 0; j < _BoneAnimNode->mNumRotationKeys; j++) {
			if (frame <= (int)_BoneAnimNode->mRotationKeys[j].mTime) {
				closectRotKeyFrame = j;
				break;
			}
		}

		int closectPosKeyFrame = 0;//FindPosition(frame, _BoneAnimNode);
		for (glm::uint j = 0; j < _BoneAnimNode->mNumPositionKeys; j++) {
			if (frame <= (int)_BoneAnimNode->mPositionKeys[j].mTime) {
				closectPosKeyFrame = j;
				break;
			}
		}

		int closectScalKeyFrame = 0; //FindScaling(frame, _BoneAnimNode);
		for (glm::uint j = 0; j < _BoneAnimNode->mNumScalingKeys; j++) {
			if (frame <= (int)_BoneAnimNode->mScalingKeys[j].mTime) {
				closectScalKeyFrame = j;
				break;
			}
		}

		_BoneAnimNode->mRotationKeys[closectRotKeyFrame].mValue = aiQuaternion(globalRot.w, globalRot.x, globalRot.y, globalRot.z);		
		_BoneAnimNode->mPositionKeys[closectPosKeyFrame].mValue = aiVector3D(globalPos.x, globalPos.y, globalPos.z);		
		_BoneAnimNode->mScalingKeys[closectScalKeyFrame].mValue = aiVector3D(globalScale.x, globalScale.y, globalScale.z);

		printf("%s: Animation Duration (%d), Current Anim Time (%d), Inserted frame at Rot(%d);Pos(%d);Scaling(%d).\n",
			NodeName.c_str(),
			(int)_scene->mAnimations[_animationToInsertInto]->mDuration,
			frame, closectRotKeyFrame, closectPosKeyFrame, closectScalKeyFrame);

	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		KinectReadNodeHeirarchyAtFrame(frame, pNode->mChildren[i], GlobalTransformation, _scene, _animationToInsertInto);
	}
}

void bMesh::KinectBoneTransform(std::vector<glm::mat4>& Transforms, aiScene* _scene)
{
	glm::mat4 Identity = glm::mat4(1.0f);
	if (_scene->HasAnimations())
	{
		KinectReadNodeHeirarchy(_scene->mRootNode, Identity, _scene);
		Transforms.clear();
		Transforms.resize(m_NumBones);

		for (glm::uint i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}
	else
	{
		printf("Error, no animations present.");
	}
}

void bMesh::KinectReadNodeHeirarchy(const aiNode* pNode,
	const glm::mat4& ParentTransform, aiScene* _scene)
{
	std::string NodeName(pNode->mName.data);
	glm::mat4 NodeTransformation = glm::mat4();
	CopyaiMat(&pNode->mTransformation, NodeTransformation);
	std::string BoneName(pNode->mName.data);

	glm::mat4 GlobalTransformation;

	std::vector<std::string>::iterator it;
	it = std::find(m_mask.begin(), m_mask.end(), NodeName);
	if (it != m_mask.end())
	{
		//printf("Mapping Kinect Joint to: %s \n", NodeName.c_str());
		glm::mat4 GlobalKinectRotation = glm::mat4(1.0);
		glm::quat _rotation = m_JointNameOrientations[NodeName].m_orientation;
		glm::quat parentRotation;
		glm::vec3 _pos;
		GlobalKinectRotation = glm::toMat4(_rotation);
		glm::decompose(NodeTransformation, glm::vec3(1.0), glm::quat(), _pos, glm::vec3(1.0), glm::vec4(1.0));
		glm::decompose(ParentTransform, glm::vec3(1.0), parentRotation, glm::vec3(1.0), glm::vec3(1.0), glm::vec4(1.0));
		glm::mat4 LocalRotation = glm::toMat4(glm::inverse(glm::conjugate(parentRotation))) * GlobalKinectRotation;
		NodeTransformation = glm::translate(_pos) * LocalRotation;
		GlobalTransformation = ParentTransform * NodeTransformation;

	}
	else
	{
		GlobalTransformation = ParentTransform * NodeTransformation;
	}


	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		glm::uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (glm::uint i = 0; i < pNode->mNumChildren; i++) {
		KinectReadNodeHeirarchy(pNode->mChildren[i], GlobalTransformation, _scene);
	}
}

void bMesh::FindScalingAtFrame(aiVector3D& Out, float frame, const aiNodeAnim* pNodeAnim)
{
	int index = frame;
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	if (pNodeAnim->mNumScalingKeys <= (int)frame)
	{
		index = 0;
	}

	const aiVector3D& Scale = pNodeAnim->mScalingKeys[index].mValue;
	Out = Scale;
}

void bMesh::FindRotationAtFrame(aiQuaternion& Out, float frame, const aiNodeAnim* pNodeAnim)
{
	int index = frame;
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	if (pNodeAnim->mNumRotationKeys <= (int)frame)
	{
		index = 0;
	}

	const aiQuaternion& currentRotationQ = pNodeAnim->mRotationKeys[index].mValue;
	Out = currentRotationQ;
	//Out = Out.Normalize();
}

void bMesh::FindPositionAtFrame(aiVector3D& Out, float frame, const aiNodeAnim* pNodeAnim)
{
	int index = frame;
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	if (pNodeAnim->mNumPositionKeys <= (int)frame)
	{
		index = 0;
	}


	const aiVector3D& Position = pNodeAnim->mPositionKeys[index].mValue;
	Out = Position;
}


void bMesh::CopyAnimationAtFrame(int frame)
{

}

void bMesh::PadImportedAnimationFrames(aiScene* _scene)
{
	// Modify existing scene & animation and "pad" it's frames.
	// Create our initial blank animation at rest pose for 1 frame
	//aiAnimation* mAnimation = new aiAnimation();
	//return;
	printf("Padding animation frames...\n");
	// Make sure we have a "scene"
	if (_scene == NULL)
	{
		printf("Scene is null at PadImportedAnimationFrames\n");
		return;
	}
		

	if (_scene->mNumAnimations == 0)
	{
		printf("mNumAnimations is 0 at PadImportedAnimationFrames\n");
		return;
	}

	if (_scene->mNumMeshes == 0)
	{
		printf("mNumMeshes is 0 at PadImportedAnimationFrames\n");
		return;
	}
	
	// _scene->mMeshes[0]->mNumBones
	for (glm::uint i = 0; i < _scene->mMeshes[0]->mNumBones; i++)
	{
		printf("Checking Bone (%s)... \n", std::string(_scene->mAnimations[0]->mChannels[i]->mNodeName.data));
		//printf("Padding Frame (%d)... \n", frame);
		// Each frame, we're checking all bones for missing frames.
		
		for (int frame = 0; frame < _scene->mAnimations[0]->mDuration; frame++)
		{			
			// Position frames is lass than the number of frames, means we're "lacking" frames.
			if (_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys < frame ||
				_scene->mAnimations[0]->mChannels[i]->mPositionKeys[frame].mTime > frame)
			{
				printf("Checking Frame (%d)... \n", frame);
				//printf("mNumPositionKeys (%d) < frame (%d), adding frame... \n", _scene->mAnimations[0]->mChannels[i]->mNumPositionKeys,
				//	frame);
				//printf("maxKeyFrames (%d)\n", _scene->mAnimations[0]->mChannels[i]->mNumPositionKeys);

				std::vector<aiVectorKey> aiVectors(_scene->mAnimations[0]->mChannels[i]->mPositionKeys,
					_scene->mAnimations[0]->mChannels[i]->mPositionKeys + _scene->mAnimations[0]->mChannels[i]->mNumPositionKeys);

				aiVectorKey _aiVector;
				_aiVector.mTime = frame;
				_aiVector.mValue = 
					_scene->mAnimations[0]->mChannels[i]->mPositionKeys[_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys - 1].mValue;

				if (_aiVector.mTime < _scene->mAnimations[0]->mChannels[i]->mPositionKeys[frame].mTime)
				{
					aiVectorKey _temp;
					_temp.mTime = 
						_scene->mAnimations[0]->mChannels[i]->mPositionKeys[_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys - 1].mTime;
					_temp.mValue = 
						_scene->mAnimations[0]->mChannels[i]->mPositionKeys[_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys - 1].mValue;
					aiVectors[_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys - 1] = _aiVector;
					_aiVector = _temp;
				}

				aiVectors.push_back(_aiVector);

				// reallocate array
				_scene->mAnimations[0]->mChannels[i]->mPositionKeys = new aiVectorKey[_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys + 1];
				for (int j = 0; j < _scene->mAnimations[0]->mChannels[i]->mNumPositionKeys + 1; j++)
				{
					_scene->mAnimations[0]->mChannels[i]->mPositionKeys[j] = aiVectors[j];
				}

				_scene->mAnimations[0]->mChannels[i]->mNumPositionKeys++;
				

			}
			
			// Rotation frames is lass than the number of frames, means we're "lacking" frames.
			if (_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys < frame ||
				_scene->mAnimations[0]->mChannels[i]->mRotationKeys[frame].mTime > frame)
			{
				std::vector<aiQuatKey> aiVectors(_scene->mAnimations[0]->mChannels[i]->mRotationKeys,
					_scene->mAnimations[0]->mChannels[i]->mRotationKeys + _scene->mAnimations[0]->mChannels[i]->mNumRotationKeys);

				aiQuatKey _aiVector;
				_aiVector.mTime = frame;
				_aiVector.mValue =
					_scene->mAnimations[0]->mChannels[i]->mRotationKeys[_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys - 1].mValue;

				if (_aiVector.mTime < _scene->mAnimations[0]->mChannels[i]->mRotationKeys[frame].mTime)
				{
					aiQuatKey _temp;
					_temp.mTime =
						_scene->mAnimations[0]->mChannels[i]->mRotationKeys[_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys - 1].mTime;
					_temp.mValue =
						_scene->mAnimations[0]->mChannels[i]->mRotationKeys[_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys - 1].mValue;
					aiVectors[_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys - 1] = _aiVector;
					_aiVector = _temp;
				}

				aiVectors.push_back(_aiVector);

				// reallocate array
				_scene->mAnimations[0]->mChannels[i]->mRotationKeys = new aiQuatKey[_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys + 1];
				for (int j = 0; j < _scene->mAnimations[0]->mChannels[i]->mNumRotationKeys + 1; j++)
				{
					_scene->mAnimations[0]->mChannels[i]->mRotationKeys[j] = aiVectors[j];
				}

				_scene->mAnimations[0]->mChannels[i]->mNumRotationKeys++;
			}
		}
	}
	printf("Done... \n");

	printf("mTime (%.3f) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mTime);
	printf("mTime (%d) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mTime);
	printf("mTime (%u) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mTime);

	printf("Value (%.3f) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mValue.x);
	printf("Value (%d) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mValue.x);
	printf("Value (%u) \n", _scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mValue.x);

	//std::cin.get();
}