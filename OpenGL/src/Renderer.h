#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak() //���Ժ궨��
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) //���OpenGL����

void GLClearError(); //���OpenGL����
bool GLLogCall(const char* function, const char* file, int line); //���OpenGL����