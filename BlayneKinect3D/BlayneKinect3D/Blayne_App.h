#pragma once

#ifndef WIN32
#include "FreetypeGL\freetypeGL.h"
#endif

class BlayneApp
{
protected:
	BlayneApp();

	void CalcFPS();

	void RenderFPS();

	float GetRunningTime();

protected:
#ifndef WIN32
	FontRenderer m_fontRenderer;
#endif
private:
	long long m_frameTime;
	long long m_startTime;
	int m_frameCount;
	int m_fps;
};