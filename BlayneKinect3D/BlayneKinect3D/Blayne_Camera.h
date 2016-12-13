#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <ATB\AntTweakBar.h>

#include "Blayne_Keys.h"
#include "Blayne_ATB.h"

class Blayne_Camera {
public:
	Blayne_Camera(int WindowWidth, int WindowHeight);
	Blayne_Camera(int WindowWidth, int WindowHeight, const glm::vec3& _pos, const glm::vec3& _target,
		const glm::vec3& _up);
	bool OnKeyboardHandler(BLAYNE_KEY _key);
	void OnMouseHandler(int x, int y);
	void OnRender();

	const glm::vec3& GetCameraPosition() const
	{
		return this->m_pos;
	}

	const glm::vec3& GetCameraLookDirection() const
	{
		return this->m_lookDir;
	}

	const glm::vec3& GetCameraUpDirection() const
	{
		return this->m_up;
	}

	void AddToATB(TwBar* bar);
private:
	glm::vec3 m_pos;
	glm::vec3 m_lookDir;
	glm::vec3 m_up;

	int m_windowWidth;
	int m_windowHeight;

	float m_AngleH;
	float m_AngleV;

	void Init();
	void Update();

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	glm::vec2 m_mousePos;
};