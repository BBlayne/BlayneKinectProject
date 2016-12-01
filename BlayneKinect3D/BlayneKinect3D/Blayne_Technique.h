#pragma once
#include <list>
#include <GL\glew.h>

class Technique
{
public:
	Technique();
	virtual ~Technique();
	virtual bool Init();
	void Enable();
protected:
	bool AddShader(GLenum ShaderType, const char* pFilename);
	bool Finalize();
	GLint GetUniformLocation(const char* pUniformName);
	GLint GetProgramParam(GLint param);
	GLuint m_shaderProg;
private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};