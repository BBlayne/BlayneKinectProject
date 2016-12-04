#include "Blayne_Camera.h"

const static float STEP_SCALE = 1.0f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 10;

Blayne_Camera::Blayne_Camera(int WindowWidth, int WindowHeight)
{
	m_windowWidth = WindowWidth;
	m_windowHeight = WindowHeight;
	m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lookDir = glm::vec3(0.0f, 0.0f, 1.0f);
	m_lookDir = glm::normalize(m_lookDir);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	Init();
}

Blayne_Camera::Blayne_Camera(int WindowWidth, int WindowHeight, const glm::vec3& _pos, const glm::vec3& _lookDir,
	const glm::vec3& _up)
{
	m_windowWidth = WindowWidth;
	m_windowHeight = WindowHeight;
	m_pos = _pos;
	//m_lookDir = glm::normalize(_lookDir);
	m_lookDir = _lookDir;
	m_up = _up;

	Init();
}

void Blayne_Camera::Init()
{
	glm::vec3 HTarget(m_lookDir.x, 0.0f, m_lookDir.z);
	HTarget = glm::normalize(HTarget);

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			
			m_AngleH = 360.0f - glm::degrees(asin(HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f + glm::degrees(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = glm::degrees(asin(-HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f - glm::degrees(asin(-HTarget.z));
		}
	}

	m_AngleV = -glm::degrees(asin(m_lookDir.y));

	m_OnUpperEdge = false;
	m_OnLowerEdge = false;
	m_OnLeftEdge = false;
	m_OnRightEdge = false;
	m_mousePos.x = m_windowWidth / 2;
	m_mousePos.y = m_windowHeight / 2;

	// glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

bool Blayne_Camera::OnKeyboardHandler(BLAYNE_KEY Key)
{
	bool Ret = false;

	switch (Key) {

	case BLAYNE_KEY_UP:
	{
		m_pos += (m_lookDir * STEP_SCALE);
		Ret = true;
	}
	break;

	case BLAYNE_KEY_DOWN:
	{
		m_pos -= (m_lookDir * STEP_SCALE);
		Ret = true;
	}
	break;

	case BLAYNE_KEY_LEFT:
	{
		glm::vec3 Left = glm::cross(m_lookDir, m_up);
		Left = glm::normalize(Left);
		Left *= STEP_SCALE;
		m_pos += Left;
		Ret = true;
	}
	break;

	case BLAYNE_KEY_RIGHT:
	{
		glm::vec3 Right = glm::cross(m_up, m_lookDir);
		Right = glm::normalize(Right);
		Right *= STEP_SCALE;
		m_pos += Right;
		Ret = true;
	}
	break;

	case BLAYNE_KEY_PAGE_UP:
		m_pos.y += STEP_SCALE;
		break;

	case BLAYNE_KEY_PAGE_DOWN:
		m_pos.y -= STEP_SCALE;
		break;

	default:
		break;
	}

	return Ret;
}

void Blayne_Camera::OnMouseHandler(int x, int y)
{
	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			//    m_AngleH -= 1.0f;
			m_OnLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) {
			//    m_AngleH += 1.0f;
			m_OnRightEdge = true;
		}
	}
	else {
		m_OnLeftEdge = false;
		m_OnRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			m_OnUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN)) {
			m_OnLowerEdge = true;
		}
	}
	else {
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}

	//Update();
}

void Blayne_Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (m_OnLeftEdge) {
		m_AngleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += EDGE_STEP;
		ShouldUpdate = true;
	}

	if (m_OnUpperEdge) {
		if (m_AngleV > -90.0f) {
			m_AngleV -= EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge) {
		if (m_AngleV < 90.0f) {
			m_AngleV += EDGE_STEP;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		//Update();
	}
}

void Blayne_Camera::Update()
{
	
	const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	glm::vec3 View(1.0f, 0.0f, 0.0f);
	View = glm::rotate(View, m_AngleH, Vaxis);
	View = glm::normalize(View);
	//View.Rotate(m_AngleH, Vaxis);
	//View.Normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 Haxis = glm::cross(Vaxis, View);
	Haxis = glm::normalize(Haxis);
	View = glm::rotate(View, m_AngleV, Haxis);
	//glm::vec3 Haxis = Vaxis.Cross(View);	
	//Haxis.Normalize();
	//View.Rotate(m_AngleV, Haxis);

	m_lookDir = View;
	m_lookDir = glm::normalize(m_lookDir);
	//m_target.Normalize();

	m_up = glm::cross(m_lookDir, Haxis);
	m_up = glm::normalize(m_up);
	//m_up = m_target.Cross(Haxis);
	//m_up.Normalize();
}

void Blayne_Camera::AddToATB(TwBar* bar)
{
	TwAddButton(bar, "Camera", NULL, NULL, "");
	TwAddVarRW(bar, "Position", TW_TYPE_BLAYNE_VECTOR3F, (void*)&m_pos, NULL);
	TwAddVarRO(bar, "Direction", TW_TYPE_DIR3F, &m_lookDir, " axisz=-z ");
}