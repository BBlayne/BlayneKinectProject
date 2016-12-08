#include "Blayne_Kinect.h"
BlayneKinect::BlayneKinect() {
	screenWidth = 1024;
	screenHeight = 768;
}

BlayneKinect::BlayneKinect(int WindowWidth, int WindowHeight) {
	screenWidth = WindowWidth;
	screenHeight = WindowHeight;

	GLubyte* mPixelBuffer = new GLubyte[screenWidth * screenHeight * 4];

	this->SetPixelBuffer(mPixelBuffer);
}

bool BlayneKinect::InitShaders()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/Passthrough.vertexshader")) {
		return false;
	}


	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/BasicTexture.fragmentshader")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	this->m_texID = GetUniformLocation("renderedTexture");	

	return true;
}

void BlayneKinect::InitRenderTarget()
{
	glGenBuffers(1, &this->m_quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	glGenTextures(1, &this->m_tex);
	glBindTexture(GL_TEXTURE_2D, this->m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->COLOUR_WIDTH,
		this->COLOUR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		m_colorBuffer);

}

bool BlayneKinect::ConvertColourBufferToTexture()
{
	glBindTexture(GL_TEXTURE_2D, this->m_tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->COLOUR_WIDTH, this->COLOUR_HEIGHT, 
		GL_BGRA, GL_UNSIGNED_BYTE, m_colorBuffer);	
	
	return true;
}

void BlayneKinect::DrawPixelBuffer()
{
	//_tex = renderedTexture;
	//glBindVertexArray(m_VAO);
	// Render on the whole framebuffer, complete from the lower left corner to the upper right
	//glViewport(0, 0, windowWidth, windowHeight);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	this->Enable();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_tex);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(this->m_texID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_quadVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);


	// Make sure the VAO is not changed from the outside    
	//glBindVertexArray(0);
}

bool BlayneKinect::KinectInit()
{
	//put initialization stuff here
	HRESULT hr;

	// get the kinect sensor
	hr = GetDefaultKinectSensor(&m_sensor);
	if (FAILED(hr))
	{
		printf("Failed to find Kinect Seneor! \n");
		exit(10);
	}

	m_sensor->Open();

	// get the depth frame source
	IDepthFrameSource* depthFrameSource;

	hr = m_sensor->get_DepthFrameSource(&depthFrameSource);

	if (FAILED(hr))
	{
		printf("Failed to find data from Depth Frame Source! \n");
		exit(10);
	}

	// Sensor gets a FrameSource which gets us a FrameReader,
	// which gets us a frame.
	// Frame information gets from a Frame Source.
	// get depth frame description
	IFrameDescription* frameDesc;
	depthFrameSource->get_FrameDescription(&frameDesc);
	hr = frameDesc->get_Width(&m_depthWidth);
	if (FAILED(hr))
	{
		printf("Failed to get_Width! \n");
		exit(10);
	}

	hr = frameDesc->get_Height(&m_depthHeight);
	if (FAILED(hr))
	{
		printf("Failed to get_Height! \n");
		exit(10);
	}

	// get the depth frame reader
	hr = depthFrameSource->OpenReader(&m_depthFrameReader);
	if (FAILED(hr))
	{
		printf("Failed to open the depth frame reader! \n");
		exit(10);
	}

	// release depthframe source
	SafeRelease(depthFrameSource);

	// allocate depth buffer
	m_depthBuffer = new Blayne_Types::uint16[m_depthWidth * m_depthHeight];

	//
	// COLOUR STUFF
	//

	// get color frame source
	IColorFrameSource* colorFrameSource;
	hr = m_sensor->get_ColorFrameSource(&colorFrameSource);
	if (FAILED(hr))
	{
		printf("Failed to get_ColorFrameSource! \n");
		exit(10);
	}

	IFrameDescription* colourFrameDesc;
	hr = colorFrameSource->get_FrameDescription(&colourFrameDesc);
	if (FAILED(hr))
	{
		printf("Failed to get frame description from colour frame src! \n");
		exit(10);
	}

	hr = colourFrameDesc->get_Width(&this->COLOUR_WIDTH);
	if (FAILED(hr)) {
		this->COLOUR_WIDTH = 1920;
		printf("Failed to get colour width from frame description! \n");
	}

	hr = colourFrameDesc->get_Height(&this->COLOUR_HEIGHT);
	if (FAILED(hr)) {
		this->COLOUR_WIDTH = 1080;
		printf("Failed to get colour width from frame description! \n");
	}

	printf("Colour Buffer set to %d by %d! \n", this->COLOUR_WIDTH, this->COLOUR_HEIGHT);


	// get color frame reader
	hr = colorFrameSource->OpenReader(&m_colorFrameReader);
	if (FAILED(hr))
	{
		printf("Failed to Open color from reader! \n");
		exit(10);
	}

	// release color frame source
	SafeRelease(colorFrameSource);

	// allocate color buffer
	//m_colorBuffer = new uint32[this->COLOUR_WIDTH * this->COLOUR_HEIGHT];
	m_colorBuffer = new GLubyte[this->COLOUR_WIDTH * this->COLOUR_HEIGHT * 4];

	// get the coordinate mapper
	hr = m_sensor->get_CoordinateMapper(&m_coordinateMapper);
	if (FAILED(hr))
	{
		printf("Failed to get_CoordinateMapper! \n");
		exit(10);
	}

	//allocate a buffer of color space points
	m_colorSpacePoints = new ColorSpacePoint[m_depthWidth * m_depthHeight];

	// Get a body frame source from which we can get our body frame reader
	IBodyFrameSource *bodyFrameSource = nullptr;
	hr = m_sensor->get_BodyFrameSource(&bodyFrameSource);
	if (FAILED(hr))
	{
		printf("Failed to get_BodyFrameSource! \n");
		exit(10);
	}

	hr = bodyFrameSource->OpenReader(&m_bodyFrameReader);
	//We're done with bodyFrameSource, so we'll release it
	SafeRelease(bodyFrameSource);

	return true;
}

void BlayneKinect::Tick(float deltaTime)
{
	//put update and drawing stuff here
	HRESULT hr;

	// Depth data stuff...
	// gettting the depthframe every tick aka the frame of the application
	IDepthFrame* depthFrame;
	hr = m_depthFrameReader->AcquireLatestFrame(&depthFrame);

	// The kinect doesn't get us as many frames as the program is requesting,
	// the program runs way faster than the kinect, so returns error codes during those
	// initial frames.
	// When asking data from kinect, first few frames will fill and THEN starts getting us
	// data.
	// Kinect 2 gets us a depth buffer that is 512 by 512 pixels.
	// there is a way to get dimensions at runtime if different type of kienct?
	if (FAILED(hr))
	{
		//printf("Failed to Acquire Latest Frame! \n");
		return;
	}

	//printf("Copying data \n");

	// actual size of buffer is 2x as big as the number of elements based in
	// number of unsigned 16 bit integers
	hr = depthFrame->CopyFrameDataToArray(m_depthWidth * m_depthHeight, m_depthBuffer);

	if (FAILED(hr))
	{
		printf("Failed to copy data! \n");
		return;
	}

	SafeRelease(depthFrame);

	// Color stuff
	IColorFrame* colorFrame;
	hr = m_colorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(hr))
	{
		return;
	}

	// get frame desc for real world app and then dynamically allocate buffer...
	hr = colorFrame->CopyConvertedFrameDataToArray(1920 * 1080 * 4 /* expects bytes */,
		static_cast<BYTE*>(m_colorBuffer), ColorImageFormat_Bgra);
	if (FAILED(hr))
	{
		return;
	}

	SafeRelease(colorFrame); // will read first frame, and then next frame "WOOPS YOU GOOFED!"

							 // color space point is a vec2 (x,y), same size as depthbuffer
	hr = m_coordinateMapper->MapDepthFrameToColorSpace(
		m_depthWidth * m_depthHeight, m_depthBuffer,
		m_depthWidth * m_depthHeight, m_colorSpacePoints);
	if (FAILED(hr))
	{
		printf("Oh no! Failed map the depth frame to color space!\n");
		return;
	}

	/*
	//copy depth data to the screen
	for (int i = 0; i < m_depthWidth * m_depthHeight; ++i)
	{
		ColorSpacePoint csp = m_colorSpacePoints[i];
		int ix = (int)csp.X;
		int iy = (int)csp.Y;

		if (ix >= 0 && ix < 1920 && iy >= 0 && iy < 1080)
			m_pixelBuffer[i] = m_colorBuffer[ix + iy * 1920];
		else
			m_pixelBuffer[i] = 0xff0000;

		//uint16 d = m_depthBuffer[i];
		//uint8 a = d & 0xff;
		//m_pixelBuffer[i] = (a << 16) | (a << 8) | a;
	}
	*/
	// copy depth data to the screen in greyscale
	/*
	for (int i = 0; i < m_depthWidth * m_depthHeight; ++i)
	{
	uint16 d = m_depthBuffer[i];
	uint8 a = d & 0xff;
	m_pixelBuffer[i] = (a << 16) | (a << 8) | a;
	}
	*/

	/*
	// copy depth data to the screen in colour
	for (int i = 0; i < 1920 * 1080; ++i)
	{
	m_pixelBuffer[i] = m_colorBuffer[i];
	}
	*/

	DrawJoints();
}

void BlayneKinect::DrawJoints()
{
	//put update and drawing stuff here
	HRESULT hr;

	if (m_bodyFrameReader != nullptr)
	{
		// Body stuff
		IBodyFrame *bodyFrame = nullptr;
		hr = m_bodyFrameReader->AcquireLatestFrame(&bodyFrame);
		if (FAILED(hr))
		{
			printf("Oh no! Failed to get latest body frame!\n");
			if (m_sensor)
			{
				BOOLEAN isSensorAvailable = false;
				hr = m_sensor->get_IsAvailable(&isSensorAvailable);
				if (SUCCEEDED(hr) && !isSensorAvailable)
				{
					printf("Sensor not available!!\n");
					return;
				}
				else
				{
					printf("Sensor is available!!\n");
					return;
				}
			}
			else {
				std::cerr << "Trouble reading the body frame.\n";
				return;
			}
		}

		IBody *bodies[BODY_COUNT] = { 0 };
		hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
		if (FAILED(hr))
		{
			printf("Oh no! Failed to refresh body data!\n");
			return;
		}

		this->processBodies(BODY_COUNT, bodies);
		//After body processing is done, we're done with our bodies so release them.
		for (unsigned int bodyIndex = 0; bodyIndex < _countof(bodies); bodyIndex++) {
			SafeRelease(bodies[bodyIndex]);
		}

		SafeRelease(bodyFrame);
	}
	else
	{
		printf("Oh no! m_bodyFrameReader is null!\n");
	}
}

void BlayneKinect::Shutdown()
{
	//put cleaning up stuff here
	SafeRelease(m_sensor);
	SafeRelease(m_depthFrameReader);
	delete[] m_depthBuffer;
	delete[] m_colorBuffer;
	SafeRelease(m_colorFrameReader);
}

void BlayneKinect::processBodies(const unsigned int &bodyCount, IBody **bodies)
{
	bool ifIsBeingTracked = false;
	for (unsigned int bodyIndex = 0; bodyIndex < bodyCount; bodyIndex++) {
		IBody *body = bodies[bodyIndex];

		//Get the tracking status for the body, if it's not tracked we'll skip it
		BOOLEAN isTracked = false;
		HRESULT hr = body->get_IsTracked(&isTracked);
		if (FAILED(hr) || isTracked == false) {
			continue;
		}

		//If we're here the body is tracked so lets get the joint properties for this skeleton
		Joint joints[JointType_Count];
		JointOrientation jointRotations[JointType_Count];
		hr = body->GetJointOrientations(_countof(joints), jointRotations);
		if (FAILED(hr)) {
			printf("Failed to get joint orientations... \n");
			return;
		}


		hr = body->GetJoints(_countof(joints), joints);
		if (SUCCEEDED(hr)) {
			ifIsBeingTracked = true;

			for (int i = 0; i < _countof(joints); i++)
			{
				const CameraSpacePoint &pos = joints[i].Position;
				ColorSpacePoint coord;
				DepthSpacePoint depthCoord;
				hr = m_coordinateMapper->MapCameraPointToColorSpace(pos, &coord);
				if (FAILED(hr))
				{
					printf("Oh no! Failed map the CameraSpacePoint to color space!\n");
					return;
				}

				hr = m_coordinateMapper->MapCameraPointToDepthSpace(pos, &depthCoord);
				if (FAILED(hr))
				{
					printf("Oh no! Failed map the CameraSpacePoint to Depth Space!\n");
					return;
				}

				int radius = 20;				

				DrawCircle(coord.X, coord.Y, radius, 1, 0, 0, 1);
				for (int j = 0; j < radius; j++)
				{
					DrawCircle(coord.X, coord.Y, j, 255, 255, 255, 1);
				}
			}

			/*
			//Let's print the head's position
			const CameraSpacePoint &headPos = joints[JointType_Head].Position;
			const CameraSpacePoint &leftHandPos = joints[JointType_HandLeft].Position;

			ColorSpacePoint coord;
			DepthSpacePoint depthCoord;

			// color space point is a vec2 (x,y), same size as color buffer
			hr = m_coordinateMapper->MapCameraPointToColorSpace(leftHandPos, &coord);
			if (FAILED(hr))
			{
				printf("Oh no! Failed map the CameraSpacePoint to color space!\n");
				return;
			}

			hr = m_coordinateMapper->MapCameraPointToDepthSpace(leftHandPos, &depthCoord);
			if (FAILED(hr))
			{
				printf("Oh no! Failed map the CameraSpacePoint to Depth Space!\n");
				return;
			}



			int radius = 20;

			DrawCircle(coord.X, coord.Y, radius, 1, 0, 0, 1);
			for (int i = 0; i < radius; i++)
			{
				DrawCircle(coord.X, coord.Y, i, 255, 255, 255, 1);
			}
			Vector4 _LeftHandRot = jointRotations[JointType_HandLeft].Orientation;
			Vector4 _HeadRot = jointRotations[JointType_Head].Orientation;

			HandState leftHandState;
			hr = body->get_HandLeftState(&leftHandState);
			if (SUCCEEDED(hr)) {
				if (leftHandState == HandState_Closed) {
					std::cout << "CLOSED HAND\n";
				}
				else if (leftHandState == HandState_Open) {
					std::cout << "OPEN HAND\n";
				}
				else if (leftHandState == HandState_Lasso) {
					std::cout << "PEW PEW HANDS\n";
				}
				else if (leftHandState == HandState_NotTracked) {
					std::cout << "HAND IS NOT TRACKED\n";
				}
				else if (leftHandState == HandState_Unknown) {
					std::cout << "HANDS STATE IS UNKNOWN\n";
				}
			}
			*/
		}
	}

	if (ifIsBeingTracked)
	{
		//printf("Failed to get joint orientations... \n");
	}
	else
	{
		printf("No bodies are being tracked.... \n");
	}
}

void BlayneKinect::PlotPixel(int x, int y,
	Blayne_Types::uint8 R,
	Blayne_Types::uint8 G,
	Blayne_Types::uint8 B,
	Blayne_Types::uint8 A)
{
	
	
	if (x < 0 || x >= this->COLOUR_WIDTH || y < 0 || y >= this->COLOUR_HEIGHT)
		return;

	//printf("Printing pixel... %d, %d: Width: %d, Height: %d \n", x, y, screenWidth, screenHeight);
	//m_pixelBuffer[x + y * this->screenWidth] = color;
	//checkImage[(i*imageWidth + j) * 4 + 0] = (GLubyte)c;
	
	m_colorBuffer[y * this->COLOUR_WIDTH * 4 + x * 4 + 0] = R;
	m_colorBuffer[y * this->COLOUR_WIDTH * 4 + x * 4 + 1] = G;
	m_colorBuffer[y * this->COLOUR_WIDTH * 4 + x * 4 + 2] = B;
	m_colorBuffer[y * this->COLOUR_WIDTH * 4 + x * 4 + 3] = A;
}

void BlayneKinect::DrawCircle(int cx, int cy, int radius, 
	Blayne_Types::uint8 R,
	Blayne_Types::uint8 G,
	Blayne_Types::uint8 B,
	int _space)
{
	float aspect_ratio = (float)this->screenWidth / this->screenHeight;
	int screenX = cx;
	int screenY = cy;
	//int screenX = aspect_ratio * cx;
	//int screenY = aspect_ratio * cy;

	//printf("Drawing circle... \n");
	for (float angle = 0.0; angle < 360; angle++)
	{
		int x = screenX - radius * cos(angle);
		int y = screenY - radius * sin(angle);
		this->PlotPixel(x, y, R, G, B, 254);

	}
}