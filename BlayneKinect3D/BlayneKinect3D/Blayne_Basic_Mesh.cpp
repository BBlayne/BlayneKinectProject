#include "Blayne_Basic_Mesh.h"
#include "Blayne_Engine_Common.h"

using namespace std;

BasicMesh::BasicMesh()
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
}

BasicMesh::BasicMesh(const BasicMesh* MeshToBeDeepCopied)
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
	// Release the previously loaded mesh (if it exists)
	Clear();

	m_orientation = MeshToBeDeepCopied->GetOrientation();

	ObjName = MeshToBeDeepCopied->ObjName;
	m_Textures = MeshToBeDeepCopied->getTextures();
	m_Entries = MeshToBeDeepCopied->getBasicMeshEntries();
	m_Positions = MeshToBeDeepCopied->m_Positions;
	m_Normals = MeshToBeDeepCopied->m_Normals;
	m_TexCoords = MeshToBeDeepCopied->m_TexCoords;
	m_Indices = MeshToBeDeepCopied->m_Indices;
	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	unsigned int NumVertices = m_Positions.size();
	unsigned int NumIndices = m_Indices.size();	

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

	for (int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Load();
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

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
}

BasicMesh::~BasicMesh()
{
	Clear();
}

void BasicMesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
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

bool BasicMesh::CreatePrism(GLfloat length, const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	/*
	-1.000000, -length, 1.000000,    // 1
	-1.000000, length, 1.000000,    // 2
	-1.000000, -length, -1.000000, // 3
	-1.000000, length, -1.000000,	// 4
	1.000000, -length, 1.000000,	// 5
	1.000000, length, 1.000000,	// 6
	1.000000, -length, -1.000000, // 7
	1.000000, length, -1.000000,	// 8
	*/

	GLfloat g_vertex_buffer_data[] = {
		// 1
		-1.000000, length, 1.000000,    // 2
		-1.000000, -length, -1.000000, // 3
		-1.000000, -length, 1.000000,    // 1
		// 2
		-1.000000, length, -1.000000,	// 4
		1.000000, -length, -1.000000, // 7
		-1.000000, -length, -1.000000, // 3
		// 3
		1.000000, length, -1.000000,	// 8
		1.000000, -length, 1.000000,	// 5
		1.000000, -length, -1.000000, // 7
		// 4
		1.000000, length, 1.000000,	// 6
		-1.000000, -length, 1.000000,    // 1
		1.000000, -length, 1.000000,	// 5
		// 5
		1.000000, -length, -1.000000, // 7
		-1.000000, -length, 1.000000,    // 1
		-1.000000, -length, -1.000000, // 3
		// 6
		-1.000000, length, -1.000000,	// 4
		1.000000, length, 1.000000,	// 6
		1.000000, length, -1.000000,	// 8
		// 7
		-1.000000, length, 1.000000,    // 2
		-1.000000, length, -1.000000,	// 4
		-1.000000, -length, -1.000000, // 3
		// 8
		-1.000000, length, -1.000000,	// 4
		1.000000, length, -1.000000,	// 8
		1.000000, -length, -1.000000, // 7
		// 9
		1.000000, length, -1.000000,	// 8
		1.000000, length, 1.000000,	// 6
		1.000000, -length, 1.000000,	// 5
		// 10
		1.000000, length, 1.000000,	// 6
		-1.000000, length, 1.000000,    // 2
		-1.000000, -length, 1.000000,    // 1
		// 11
		1.000000, -length, -1.000000, // 7
		1.000000, -length, 1.000000,	// 5
		-1.000000, -length, 1.000000,    // 1
		// 12
		-1.000000, length, -1.000000,	// 4
		-1.000000, length, 1.000000,    // 2
		1.000000, length, 1.000000	// 6
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
		// 1
		0.7500, 1 - 0.9999, // 1
		0.5000, 1 - 0.7500, // 2
		0.7500, 1 - 0.7500, // 3
		// 2
		0.5000, 1 - 0.9999, // 4
		0.2500, 1 - 0.7500, // 5
		0.5000, 1 - 0.7500, // 
		// 3
		0.2500, 1 - 0.9999, // 6
		0.0001, 1 - 0.7500, // 7
		0.2500, 1 - 0.7500, // 5
		// 4
		0.2500, 1 - 0.2500, // 8
		0.5000, 1 - 0.5000, // 9
		0.2500, 1 - 0.5000, // 10
		// 5
		0.2500, 1 - 0.7500, // 5
		0.5000, 1 - 0.5000, // 9
		0.5000, 1 - 0.7500, // 2
		// 6
		0.5000, 1 - 0.0001, // 11
		0.2500, 1 - 0.2500, // 8
		0.2500, 1 - 0.0001, // 12
		// 7
		0.7500, 1 - 0.9999, // 1
		0.5000, 1 - 0.9999, // 4
		0.5000, 1 - 0.7500, // 2
		// 8
		0.5000, 1 - 0.9999, // 4
		0.2500, 1 - 0.9999, // 6
		0.2500, 1 - 0.7500, // 5
		// 9
		0.2500, 1 - 0.9999, // 6
		0.0001, 1 - 0.9999, // 13
		0.0001, 1 - 0.7500, // 7
		// 10
		0.2500, 1 - 0.2500, // 8
		0.5000, 1 - 0.2500, // 14
		0.5000, 1 - 0.5000, // 9
		// 11
		0.2500, 1 - 0.7500, // 5
		0.2500, 1 - 0.5000, // 10
		0.5000, 1 - 0.5000, // 9
		// 12
		0.5000, 1 - 0.0001, // 11
		0.5000, 1 - 0.2500, // 14
		0.2500, 1 - 0.2500  // 8
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
	BasicMeshEntry entry;
	m_Entries.push_back(entry);
	Texture* tex = new Texture(GL_TEXTURE_2D, std::string("CubeUVs - Copy.png").c_str());
	m_Textures.push_back(tex);

	if (!m_Textures[0]->Load()) {
		printf("Error loading texture '%s'\n", std::string("CubeUVs - Copy.png").c_str());
		delete m_Textures[0];
		m_Textures[0] = NULL;
		return false;
	}
	else {
		printf("%d - loaded texture '%s'\n", 0, std::string("CubeUVs - Copy.png").c_str());
	}

	// Populate the index buffer
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(Positions, TexCoords, Normals, Indices, indexed_vertices, indexed_uvs,
		indexed_normals);

	printf("NumFaces %d, numVerts %d, numNormals %d, numIndices %d \n", Positions.size() / 3,
		Positions.size(), Normals.size(), Indices.size());

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

bool BasicMesh::LoadMesh(const string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
		ObjName = Filename;
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}

bool BasicMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	vector<glm::vec3> Positions;
	vector<glm::vec3> Normals;
	vector<glm::vec2> TexCoords;
	vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	// Count the number of vertices and indices
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
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(paiMesh, Positions, Normals, TexCoords, Indices);
	}

	m_Positions = Positions;
	m_Normals = Normals;
	m_TexCoords = TexCoords;
	m_Indices = Indices;

	if (!InitMaterials(pScene, Filename)) {
		return false;
	}

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

void BasicMesh::InitMesh(const aiMesh* paiMesh,
	vector<glm::vec3>& Positions,
	vector<glm::vec3>& Normals,
	vector<glm::vec2>& TexCoords,
	vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

bool BasicMesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
	// Extract the directory part from the file name
	string::size_type SlashIndex = Filename.find_last_of("/");
	string Dir;

	if (SlashIndex == string::npos) {
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
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				string FullPath = Dir + "/" + p;

				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}
		else
		{
			printf("Error: Texture count is zero.\n");
		}
	}

	return Ret;
}

void BasicMesh::Render()
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
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

// particles?
void BasicMesh::Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			NumInstances,
			m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}