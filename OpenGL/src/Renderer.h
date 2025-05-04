#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak() //断言宏定义
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) //检查OpenGL错误

void GLClearError(); //清除OpenGL错误
bool GLLogCall(const char* function, const char* file, int line); //检查OpenGL错误