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
#include <chrono>

using namespace std::chrono;
typedef steady_clock Clock;

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
#include "Blayne_3D_Math.h"
#include "Blayne_Pipeline.h"
#include "Blayne_Basic_Mesh.h"
#include "bMesh.h"
#include "BlayneRenderToTexture.h"
#include "Blayne_Kinect.h"
#include "BlayneSkinningTechnique.h"


int gGLMajorVersion = 0;
/* 
*	Globals for the Main program and App.
*/
// Default Window Size 
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef enum { CHEB, UNITYCHAN, STEVE, ADRIAN } MESH_TYPE;

typedef enum { TPOSE, FREE, VIEW, INSERT, PLAYER, KINECT_COL } ANIMATION_MODE;

#define imageWidth 1024
#define imageHeight 1024
//static GLubyte checkImage[imageHeight][imageWidth][4];
const int channels = 4;
static GLubyte* checkImage;
static GLuint texName;
bool insertedKeyFrame = false;
bool isInsertingKeyFrame = false;
auto lastTime = Clock::now();

// Stuff for key frames
int minFrames = 0;
int maxFrames = 1;
int frameVal = 0;
int maxFramesVal = 0;
int prevMaxFramesVal = 0;

// Function called to copy the content of a std::string (souceString) handled 
// by the AntTweakBar library to destinationClientString handled by our application
void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	// Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
	destinationClientString = sourceLibraryString;
}

std::vector<std::string> g_BarTitles;

// Callback function called by AntTweakBar to set the "EditTitle" std::string variable
void TW_CALL SetBarTitleCB(const void *value, void *clientData)
{
	const std::string *newTitle = (const std::string *)(value);
	int barIndex = *(int *)(&clientData);   // clientData stores the bar index

											// Stores the new bar title
	g_BarTitles[barIndex] = *newTitle;

	// Create the def command to change the bar label (ie., its title)
	std::stringstream def;
	def << "bar_" << barIndex << " label=`" << g_BarTitles[barIndex] << "`";
	// Execute the command
	TwDefine(def.str().c_str());
}

// Callback function called by AntTweakBar to get the "EditTitle" std::string variable
void TW_CALL GetBarTitleCB(void *value, void *clientData)
{
	std::string *destStringPtr = (std::string *)(value);
	int barIndex = *(int *)(&clientData);   // clientData stores the bar index
	std::string title = g_BarTitles[barIndex];

	// Do not assign destStringPtr directly (see TwCopyStdStringToLibrary doc for explanation):
	// Use TwCopyStdStringToLibrary to copy the bar title string to AntTweakBar
	TwCopyStdStringToLibrary(*destStringPtr, title);
}

void TW_CALL InsertKeyFrameButton(void *clientData)
{
	ANIMATION_MODE _mode = *(ANIMATION_MODE*)clientData;
	if (_mode == ANIMATION_MODE::INSERT)
		isInsertingKeyFrame = true;
}

void makeCheckImage()
{
	checkImage = new GLubyte[imageHeight*imageWidth*channels];
	int i, j, c;

	for (i = 0; i < imageHeight; i++) {
		for (j = 0; j < imageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[(i*imageWidth + j)*4+0] = (GLubyte)c;
			checkImage[(i*imageWidth + j)*4+1] = (GLubyte)c;
			checkImage[(i*imageWidth + j)*4+2] = (GLubyte)c;
			checkImage[(i*imageWidth + j)*4+3] = (GLubyte)255;
		}
	}
}

void initImage()
{

	/* ... */

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth,
		imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
}

class BlayneKinect3D : public IBlayneCallbacks, public BlayneApp
{
private:
	std::vector<BlayneBasicLightingTechnique> m_LightingTechs;
	BlayneBasicLightingTechnique m_LightingTech;
	BlayneBasicLightingTechnique m_KinectTectureTech;
	// Shader/Lighting info for a skinned mesh
	SkinningTechnique* m_SkinningTech;
	Blayne_Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	// Basic Meshes Array?
	std::vector<BasicMesh*> m_BasicMeshes;
	std::vector<bMesh*> m_SkinnedMeshes; // Also maybe a skeleton?
	PersProjInfo m_persProjInfo;
	OrthoProjInfo m_orthoProjInfo;
	std::vector<Blayne_Pipeline> m_pipelines;
	Blayne_Pipeline m_pipeline;
	Blayne_ATB m_atb;
	// Mesh type? Some weird Enum if bunch of meshes, probably ignore?
	MESH_TYPE m_currentMesh;
	float m_rotationSpeed; // camera? Or Mesh's we're looking at?
	TwBar *bar;
	RenderToTexture m_RenderToTexturer;
	BlayneKinect* m_KinectObj;
	float deltaTime;
	//bool InitBasicMesh(std::string _path, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	//void RenderBasicMesh(int _whichPipe, int _whichMesh);
	glm::vec3 m_objRotationEuler = glm::vec3(0, 0, 0);
	ANIMATION_MODE m_currentMode = ANIMATION_MODE::TPOSE;
	aiAnimation* m_CurrentSelectAnim;
	int m_currentlySelectedAnimation = 0;
	int m_maxAnimation;
	TwType animationsType;
public:
	BlayneKinect3D() { 
		m_pGameCamera = NULL;

		m_directionalLight.Name = "DirLight1";
		m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.66f;
		m_directionalLight.DiffuseIntensity = 1.0f;
		m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

		// Default Pipeline
		float aspect_ratio = (float)WINDOW_WIDTH / WINDOW_HEIGHT;

		m_persProjInfo.FOV = 45.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.aspect_ratio = aspect_ratio;
		m_persProjInfo.zNear = 0.1f;
		m_persProjInfo.zFar = 1000.0f;

		float zoom = 1.0f;
		
		m_orthoProjInfo.l = -1.0f * zoom;
		m_orthoProjInfo.r = 1.0f * zoom;
		m_orthoProjInfo.b = -1.0f / aspect_ratio * zoom;
		m_orthoProjInfo.t = 1.0f / aspect_ratio * zoom;
		m_orthoProjInfo.n = -0.1f;
		m_orthoProjInfo.f = 1000.0f;

		m_pipeline.SetPerspectiveProj(m_persProjInfo);
		m_pipeline.SetOrthographicProj(m_orthoProjInfo);
		m_pipelines.push_back(m_pipeline);

		// Set Current Mesh?
		// What's this do?
		glGetIntegerv(GL_MAJOR_VERSION, &gGLMajorVersion); 
	}

	~BlayneKinect3D()
	{
		SAFE_DELETE(m_pGameCamera);
	}



	bool Init()
	{
		m_KinectObj = new BlayneKinect(WINDOW_WIDTH, WINDOW_HEIGHT);

		if (!m_atb.Init()) {
			return false;
		}

		TwCopyStdStringToClientFunc(CopyStdStringToClient);

		glm::vec3 Pos(0.0f, 4.0f, -8.0f);
		glm::vec3 LookAt(0.0f, 4.0f, 0.0f);
		glm::vec3 Up(0.0f, 1.0f, 0.0f);

		initImage();


		m_pGameCamera = new Blayne_Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, LookAt, Up);
		if (!m_LightingTech.Init()) {
			printf("Error initializing the lighting technique\n");
			OGLDEV_ERROR("Error initializing the lighting technique\n");			
			return false;
		}

		if (!m_KinectTectureTech.Init2())
		{
			printf("Error initializing the m_KinectTectureTech \n");
			OGLDEV_ERROR("Error initializing the m_KinectTectureTech\n");
			return false;
		}

		m_KinectTectureTech.Enable();
		m_KinectTectureTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_KinectTectureTech.SetDirectionalLight(m_directionalLight);
		m_KinectTectureTech.SetMatSpecularIntensity(0.0f);
		m_KinectTectureTech.SetMatSpecularPower(0);

		m_LightingTech.Enable();
		printf("m_LightingTech enabled\n");

		m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_LightingTech.SetDirectionalLight(m_directionalLight);
		m_LightingTech.SetMatSpecularIntensity(0.0f);
		m_LightingTech.SetMatSpecularPower(0);

		m_SkinningTech = new SkinningTechnique();
		if (!m_SkinningTech->Init())
		{
			printf("Error initializing the lighting technique (SkinningTechnique) \n");
			return false;
		}
		m_SkinningTech->Enable();
		printf("m_SkinningTech enabled\n");
		m_SkinningTech->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_SkinningTech->SetDirectionalLight(m_directionalLight);
		m_SkinningTech->SetMatSpecularIntensity(0.0f);
		m_SkinningTech->SetMatSpecularPower(0);

		// Load Meshes & Orientations
		
		if (!this->InitBasicMesh("StandardCube.fbx",
			glm::vec3(0.0f, -0.1f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 0.1f, 10.0f)))
			return false;
			
		// Dave.fbx, BasicCheb.fbx
		if (!this->InitBasicMesh("George.fbx",
			glm::vec3(0.0f, 0.0f, 0.0f),
			m_objRotationEuler,
			glm::vec3(1.0f, 1.0f, 1.0f)))
			return false;
		/*
		if (!this->InitSkinnedMesh("Dave.fbx",
			glm::vec3(0.0f, 0.0f, 0.0f),
			m_objRotationEuler,
			glm::vec3(1.0f, 1.0f, 1.0f)))
		{
			printf("Error loading Dave\n");
			return false;
		}
		*/

		if (!this->InitSkinnedMesh("George.fbx",
			glm::vec3(0.0f, 0.0f, 0.0f),
			m_objRotationEuler,
			glm::vec3(1.0f, 1.0f, 1.0f)))
		{
			printf("Error loading Dave\n");
			return false;
		}
			

		// TwBar stuff.
		bar = TwNewBar("Blayne's Kinect v2.0 3D App.");

		TwEnumVal AnimationModes[] = {
			{ TPOSE, "TPose" },
			{ FREE, "Free" },
			{ VIEW, "View" },
			{ INSERT, "Insert" },
			{ PLAYER, "Player" },
			{ KINECT_COL, "Kinect" }
		};

		TwType AnimTwType = TwDefineEnum("AnimationMode", AnimationModes, 6);
		// Link it to the tweak bar
		TwAddVarRW(bar, "Anim Mode:", AnimTwType, &m_currentMode, NULL);

		// The second parameter is an optional name
		TwAddSeparator(bar, "", NULL);

		// Animations
		animationsType = TwDefineEnum("SeasonType", NULL, 0);

		std::string defaultAnims;
		m_maxAnimation = 0;
		if (m_SkinnedMeshes[0]->getScene()->HasAnimations())
		{
			defaultAnims = " enum='";
			m_maxAnimation = m_SkinnedMeshes[0]->getScene()->mNumAnimations;
			int i = 0;
			for ( ; i < m_maxAnimation; i++)
			{
				defaultAnims.append(std::to_string(i) + " {").append(m_SkinnedMeshes[0]->getScene()->mAnimations[i]->mName.data).append("}, ");
			}

			defaultAnims.append(std::to_string(i) + " { New Animation }' ");
		}
		else
		{
			defaultAnims = " enum='0 { New Animation }' ";
			// SkinnedMesh Create new Blank animation with default T-Pose keyframe at frame 0
		}

		TwAddVarRW(bar, "Animations", animationsType, &m_currentlySelectedAnimation,
			defaultAnims.c_str());

		TwAddSeparator(bar, "", NULL);

		m_pGameCamera->AddToATB(bar);
		TwAddSeparator(bar, "", NULL);
		TwAddVarRW(bar, "ObjRotation", TW_TYPE_BLAYNE_VECTOR3F, (void*)&m_objRotationEuler, NULL);
		TwAddSeparator(bar, "", NULL);
		m_directionalLight.AddToATB(bar);
		float refresh = 0.1f;
		TwSetParam(bar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);
		TwAddSeparator(bar, "", NULL);
		TwAddButton(bar, "InsertKeyFrame", InsertKeyFrameButton, &m_currentMode, " label='Insert Key Frame' ");
		// Current Frame selects the current available frame to view & insert
		TwAddVarRW(bar, "frame", TW_TYPE_INT32, &frameVal,
			" label='Current Frame' step=1 ");
		TwAddVarRW(bar, "maxframes", TW_TYPE_INT32, &maxFramesVal,
			" label='Set Max Frames' step=1 ");

		maxFrames = m_SkinnedMeshes[0]->getScene()->mAnimations[m_currentlySelectedAnimation]->mDuration;
		maxFramesVal = maxFrames + 1;
		TwSetParam(bar, "frame", "min", TW_PARAM_INT32, 1, &minFrames);
		TwSetParam(bar, "frame", "max", TW_PARAM_INT32, 1, &maxFrames);
		minFrames = 1;
		TwSetParam(bar, "maxframes", "min", TW_PARAM_INT32, 1, &maxFrames);
		TwSetParam(bar, "maxframes", "max", TW_PARAM_INT32, 1, &maxFrames);

		// Message added to the help bar.
		TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with OGLDEV.' "); 
		TwAddVarRO(bar, "GL Major Version", TW_TYPE_INT32, &gGLMajorVersion, " label='Major version of GL' ");

		/*
		if (!m_RenderToTexturer.InitFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, texName))
		{
			printf(" Did not init frame buffer?\n");
			return false;
		}
		
		if (!m_RenderToTexturer.InitShaders())
		{
			printf(" Did not init shaders for frame buffer?\n");
			return false;
		}
		*/

		if (!m_KinectObj->KinectInit())
		{
			printf(" Did not init kinect?\n");
			return false;
		}

		
		if (!m_KinectObj->InitShaders())
		{
			printf(" Did not init kinect shaders?\n");
			return false;
		}
		

		m_KinectObj->InitRenderTarget();
		m_KinectObj->setBoneNameJointOrientations();
		m_KinectObj->setMask();

		lastTime = Clock::now();

		return true;
	}

	virtual void RenderSceneCB()
	{
		// Kinect
		m_KinectObj->TickForJointsInfo(deltaTime);
			//m_RenderToTexturer.RenderToFrameBuffer();
		// Handle camera being rotated via user edging
		// the mouse along the edge of the screen.
		//m_pGameCamera->OnRender();      
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		m_LightingTech.Enable();

		// Set the camera position for the lighting, and pass our directional light
		// to our lighting system.
		m_LightingTech.SetEyeWorldPos(m_pGameCamera->GetCameraPosition());
		m_LightingTech.SetDirectionalLight(m_directionalLight);

		// Pass our camera object to our pipeline object.
		m_pipelines[0].SetCamera(*m_pGameCamera);

		// Refresh orientations & call rendering
		this->RenderBasicMesh(0, 0);
		this->RenderSkinnedMesh(0, 0);
	
		//  RenderFPS();     

			//m_RenderToTexturer.RenderToScreen();
			//m_RenderToTexturer.Render(texName);

		


		CalcFPS();
		//calculate delta time
		const auto now = Clock::now();
		const auto duration = duration_cast<microseconds>(now - lastTime);
		deltaTime = duration.count() / 1000000.0f;

		lastTime = now;

		if (m_currentMode == KINECT_COL)
		{
			m_KinectTectureTech.Enable();
			m_KinectObj->Tick(deltaTime);
			m_KinectObj->ConvertColourBufferToTexture();
			m_KinectObj->DrawPixelBuffer();
		}
		
		//printf("DT: %.3f \n", deltaTime);
		//m_KinectObj->ConvertColourBufferToTexture();
		//m_KinectObj->DrawPixelBuffer();

		BlayneBackendSwapBuffers();
	}

	virtual void KeyboardCB(BLAYNE_KEY BlayneKey, BLAYNE_KEY_STATE BlayneKeyState)
	{
		if (BlayneKeyState == BLAYNE_KEY_STATE_PRESS)
		{
			if (m_atb.KeyboardCB(BlayneKey))
			{
				return;
			}
		}

		switch (BlayneKey) {
			case BLAYNE_KEY_A:
			{
				break;
			}
			case BLAYNE_KEY_B:
			{
				break;
			}
			case BLAYNE_KEY_C:
			{
				break;
			}
			case BLAYNE_KEY_ESCAPE:
			case BLAYNE_KEY_q:
			{
				BlayneBackendLeaveMainLoop();
				break;
			}
			default:
				m_pGameCamera->OnKeyboardHandler(BlayneKey);
		}
	}

	virtual void MouseClickHandlerCB(BLAYNE_MOUSE Button, BLAYNE_KEY_STATE State, int x, int y)
	{
		m_atb.MouseCB(Button, State, x, y);
	}

	virtual void PassiveMouseHandlerCB(int x, int y)
	{
		if (!m_atb.PassiveMouseCB(x, y))
		{
			m_pGameCamera->OnMouseHandler(x, y);
		}
	}

	void Run()
	{
		BlayneBackendRun(this);
	}

	bool InitBasicMesh(std::string _path, glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale)
	{
		BasicMesh* mMesh = new BasicMesh();
		if (!mMesh->LoadMesh(_path)) {
			printf(" Did not load mesh?\n");
			return false;
		}
		// Set object orientations here.
		mMesh->GetOrientation().m_translation = _pos;
		mMesh->GetOrientation().m_rotation = _rot;
		mMesh->GetOrientation().m_scale = _scale;
		m_BasicMeshes.push_back(mMesh);

		return true;
	}

	bool InitSkinnedMesh(std::string _path, glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale)
	{
		bMesh* mMesh = new bMesh();
		if (!mMesh->LoadMesh(_path)) {
			printf(" Did not load mesh?\n");
			return false;
		}
		maxFrames = mMesh->getScene()->mAnimations[0]->mDuration - 1;
		// Set object orientations here.
		m_SkinnedMeshes.push_back(mMesh);
		return true;
	}

	void RenderBasicMesh(int _whichPipe, int _whichMesh)
	{
		m_LightingTech.Enable();
		//printf("%d, %d \n", _whichPipe, _whichMesh);
		// Refresh object orientations here.
		//m_pipelines[0].Orient(m_BasicMeshes[_whichMesh]->GetOrientation());
		//m_pipelines[_whichPipe].Orient(m_BasicMeshes[_whichMesh]->GetOrientation());
		m_pipelines[_whichPipe].Orient(m_BasicMeshes[_whichMesh]->GetOrientation());
		if (_whichMesh == 1)
		{
			//m_pipelines[_whichPipe].Rotate(0, 0, 90 * deltaTime);
			//m_BasicMeshes[_whichMesh]->GetOrientation().m_rotation = m_objRotationEuler;
			m_pipelines[_whichPipe].Rotate(m_objRotationEuler);
			//printf("Orientation of Cheb (z): %f \n", m_BasicMeshes[_whichMesh]->GetOrientation().m_rotation.z);
		}
			
		
		
		//printf("%f, %f, %f \n", m_BasicMeshes[_whichMesh]->GetOrientation().m_translation.x,
		//	m_BasicMeshes[_whichMesh]->GetOrientation().m_translation.y,
		//	m_BasicMeshes[_whichMesh]->GetOrientation().m_translation.z);
		m_LightingTech.SetWVP(m_pipelines[0].GetWVPTrans());
		//m_LightingTech.SetWorldMatrix(m_pipelines[0].GetWorldTrans());
		// Render Mesh
		m_BasicMeshes[_whichMesh]->Render();
	}

	void RenderSkinnedMesh(int _whichPipe, int _whichMesh)
	{
		m_SkinningTech->Enable();
		std::vector<glm::mat4> transforms;
		
		float runningTime = GetRunningTime();

		if (m_currentlySelectedAnimation < m_SkinnedMeshes[0]->getScene()->mNumAnimations)
		{
			maxFrames = m_SkinnedMeshes[0]->getScene()->mAnimations[m_currentlySelectedAnimation]->mDuration;
			maxFramesVal = maxFrames;
			TwSetParam(bar, "frame", "max", TW_PARAM_INT32, 1, &maxFrames);
			TwSetParam(bar, "maxframes", "max", TW_PARAM_INT32, 1, &maxFrames);
			TwRefreshBar(bar);
		}


		if (m_currentlySelectedAnimation == m_maxAnimation)
		{
			// Can I change the name...?
			AddNewAnimationToScene(); // testing this out

			m_currentMode = ANIMATION_MODE::INSERT;
			m_SkinnedMeshes[_whichMesh]->m_mask = m_KinectObj->getMask();
			m_SkinnedMeshes[_whichMesh]->m_JointNameOrientations = m_KinectObj->getJointNameOrientations();
			m_SkinnedMeshes[_whichMesh]->KinectBoneTransform(transforms,
				(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene());
		}
		else
		{
			if (m_currentMode == ANIMATION_MODE::TPOSE)
			{
				// Render Basic Mesh
				RenderBasicMesh(0, 1);
				return;
			}
			else if (m_currentMode == ANIMATION_MODE::FREE)
			{
				m_SkinnedMeshes[_whichMesh]->m_mask = m_KinectObj->getMask();
				m_SkinnedMeshes[_whichMesh]->m_JointNameOrientations = m_KinectObj->getJointNameOrientations();
				m_SkinnedMeshes[_whichMesh]->KinectBoneTransform(transforms,
					(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene());
			}
			else if (m_currentMode == ANIMATION_MODE::INSERT)
			{
				maxFrames = m_SkinnedMeshes[0]->getScene()->mAnimations[m_currentlySelectedAnimation]->mDuration;
				maxFramesVal = maxFrames;
				TwSetParam(bar, "frame", "max", TW_PARAM_INT32, 1, &maxFramesVal);
				TwSetParam(bar, "maxframes", "max", TW_PARAM_INT32, 1, &maxFramesVal);
				TwRefreshBar(bar);
				if (m_KinectObj->getBothHandsClosed() || isInsertingKeyFrame)
				{
					if (!insertedKeyFrame)
					{
						// Both hands are closed, insert key frame.
						printf("Hands are closed. \n");
						insertedKeyFrame = true;
						m_SkinnedMeshes[_whichMesh]->m_mask = m_KinectObj->getMask();
						m_SkinnedMeshes[_whichMesh]->m_JointNameOrientations = m_KinectObj->getJointNameOrientations();
						// We pass the frame in which we wish to insert our new key frame animation
						m_SkinnedMeshes[_whichMesh]->KinectBoneTransformAtFrame(frameVal, transforms,
							(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene(), m_currentlySelectedAnimation);
					}
					else
					{
						isInsertingKeyFrame = false;

						m_SkinnedMeshes[_whichMesh]->m_mask = m_KinectObj->getMask();
						m_SkinnedMeshes[_whichMesh]->m_JointNameOrientations = m_KinectObj->getJointNameOrientations();
						m_SkinnedMeshes[_whichMesh]->KinectBoneTransform(transforms,
							(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene());
					}
				}
				else
				{
					if (insertedKeyFrame)
					{
						insertedKeyFrame = false;
					}

					m_SkinnedMeshes[_whichMesh]->m_mask = m_KinectObj->getMask();
					m_SkinnedMeshes[_whichMesh]->m_JointNameOrientations = m_KinectObj->getJointNameOrientations();
					m_SkinnedMeshes[_whichMesh]->KinectBoneTransform(transforms,
						(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene());
				}
			}
			else if (m_currentMode == ANIMATION_MODE::VIEW)
			{
				// Viewing inserted frames
				m_SkinnedMeshes[_whichMesh]->BoneTransformAtFrame(frameVal, transforms,
					(aiScene*)m_SkinnedMeshes[_whichMesh]->getScene(), m_currentlySelectedAnimation);
			}
			else if (m_currentMode == ANIMATION_MODE::PLAYER)
			{
				m_SkinnedMeshes[_whichMesh]->BoneTransform(runningTime, transforms, m_currentlySelectedAnimation);
			
			}
		}

		for (int i = 0; i < transforms.size(); i++)
		{
			m_SkinningTech->SetBoneTransform(i, transforms[i]);
		}
		m_pipelines[_whichPipe].Rotate(m_objRotationEuler);
		m_pipelines[_whichPipe].Scale(1.0, 1.0, 1.0);
		m_pipelines[_whichPipe].WorldPos(0.0, 0.0, 0.0);
		m_SkinningTech->SetEyeWorldPos(m_pGameCamera->GetCameraPosition());		
		m_SkinningTech->SetWVP(m_pipelines[_whichPipe].GetWVPTrans());
		m_SkinningTech->SetWorldMatrix(m_pipelines[_whichPipe].GetWorldTrans());
		m_SkinnedMeshes[_whichMesh]->Render();
	}

	void AddNewAnimationToScene()
	{
		m_SkinnedMeshes[0]->CreateAnimation();
		std::string defaultAnims;
		m_maxAnimation = 0;

		defaultAnims = "";
		m_maxAnimation = m_SkinnedMeshes[0]->getScene()->mNumAnimations;
		int i = 0;
		for (; i < m_maxAnimation; i++)
		{
			defaultAnims.append(std::to_string(i) + " {").append(m_SkinnedMeshes[0]->getScene()->mAnimations[i]->mName.data).append("}, ");
		}

		defaultAnims.append(std::to_string(i) + " { New Animation } ");

		TwSetParam(bar, "Animations", "enum", TW_PARAM_CSTRING, 1, defaultAnims.c_str());
		TwRefreshBar(bar);
	}
};

int main(int argc, char** argv)
{
	BlayneBackendInit(BLAYNE_BACKEND_TYPE_GLFW, argc, argv, true, false);
	if (!BlayneBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "BlayneKinect3D")) {
		BlayneBackendTerminate();
		std::cin.get();
		return 1;
	}

	BlayneKinect3D* blayneKinect3DApp = new BlayneKinect3D();

	if (!blayneKinect3DApp->Init())
	{
		printf("Something went wrong init'ing program.\n");
		delete blayneKinect3DApp;
		BlayneBackendTerminate();
		std::cin.get();
		return 1;
	}

	blayneKinect3DApp->Run();

	BlayneBackendTerminate();
	//std::cin.get();
	return 0;
}