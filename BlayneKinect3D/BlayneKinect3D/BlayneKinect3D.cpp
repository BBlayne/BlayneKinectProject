#define SNPRINTF _snprintf_s
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "GL\freeglut.h"
#include <Windows.h>

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace glm;

// OpenGL Tutorial Includes
#include "ogl\shader.hpp"

#include "assimp\Importer.hpp" // C++ importer interface
#include "assimp\scene.h"// Output data structure
#include "assimp\postprocess.h"// Post processing flags

// XML Pugixml
#include "pugixml\pugixml.hpp"

#include "ImageMagick\Magick++.h"
//#include "bTexture.h"
//#include "texture.hpp"
//#include "Globals.h"
//#include "bMesh.h"

// ATB
#include "Blayne_ATB.h"

// Dependencies
#include "Blayne_Camera.h"
#include "Blayne_App.h"
#include "Blayne_Backend.h"
#include "Blayne_Basic_Lighting.h"
#include "Blayne_Engine_Common.h"

/* 
*	Globals for the Main program and App.
*/
// Default Window Size 
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef enum { CHEB, UNITYCHAN, STEVE, ADRIAN } MESH_TYPE;

class BlayneKinect3D : public IBlayneCallbacks, public BlayneApp
{
private:
	BlayneBasicLightingTechnique m_LightingTech;
	Blayne_Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	// Basic Meshes Array?
	//PersProjInfo
	Blayne_ATB m_atb;
	// Mesh type? Some weird Enum if bunch of meshes, probably ignore?
	MESH_TYPE m_currentMesh;
	float m_rotationSpeed; // camera? Or Mesh's we're looking at?
	TwBar *bar;
public:
	BlayneKinect3D() { 
	
	}

	~BlayneKinect3D()
	{
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		if (!m_atb.Init()) {
			return false;
		}

		glm::vec3 Pos(0.0f, 0.0f, 0.0f);
		glm::vec3 LookAt(0.0f, -1.0f, 1.0f);
		glm::vec3 Up(0.0f, 1.0f, 0.0f);

		m_pGameCamera = new Blayne_Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, LookAt, Up);

		if (!m_LightingTech.Init()) {
			OGLDEV_ERROR("Error initializing the lighting technique\n");
			return false;
		}

		m_LightingTech.Enable();

		m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_LightingTech.SetDirectionalLight(m_directionalLight);
		m_LightingTech.SetMatSpecularIntensity(0.0f);
		m_LightingTech.SetMatSpecularPower(0);

		// Load Meshes Here.
		// Set Orientation of one of them?
	}

	void RenderScene()
	{

	}

	void KeyboardHandler()
	{

	}

	void MouseClickHandler()
	{

	}

	void PassiveMouseHandler()
	{

	}
};

int main(int argc, char** argv)
{
	

	return 0;
}