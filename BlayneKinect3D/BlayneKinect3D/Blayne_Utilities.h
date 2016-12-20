#pragma once
//#include "GL\glew.h"
//#include "GLFW\glfw3.h"
//#include "GL\freeglut.h"
#ifndef WIN32
#include "BlayneUniStd.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>

#include "Blayne_Keys.h"
#include "Blayne_Types.h"

using namespace std;

static const char* BlayneKeyToStr[] =
{
	"Space",
	"Apostrophe",
	"Plus",
	"Comma",
	"Minus",
	"Period",
	"Slash",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Semicolon",
	"Equal Sign",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Left Bracket",
	"Backslash",
	"Right Bracket",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"Escape",
	"Enter",
	"Tab",
	"Backspace",
	"Insert",
	"Delete",
	"Right",
	"Left",
	"Down",
	"Up",
	"Page up",
	"Page down",
	"Home",
	"End",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"Undefined"
};

static const char* BlayneMouseButtonToStr[] =
{
	"LMB",
	"MMB",
	"RMB",
	"UndefinedMB"
};

static const char* BlayneButtonStateToStr[] =
{
	"Press",
	"Release"
};

static const char * getBlayneKeyToStr(int enumVal)
{
	return BlayneKeyToStr[enumVal];
}

static const char * getBlayneMouseButtonToStr(int enumVal)
{
	return BlayneMouseButtonToStr[enumVal];
}

static const char * getBlayneButtonStateToStr(int enumVal)
{
	return BlayneButtonStateToStr[enumVal];
}



bool ReadFile(const char* fileName, string& outFile);

void OgldevError(const char* pFileName, Blayne_Types::uint32 line, const char* pError);
void OgldevFileError(const char* pFileName, Blayne_Types::uint32 line, const char* pFileError);

#define OGLDEV_ERROR(Error) OgldevError(__FILE__, __LINE__, Error);
#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifdef WIN32
#define SNPRINTF _snprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#if (_MSC_VER == 1900)
#elif (_MSC_VER == 1800)
#else
float fmax(float a, float b);
#endif
#else
#define SNPRINTF snprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define INVALID_MATERIAL 0xFFFFFFFF

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long GetCurrentTimeMillis();

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)