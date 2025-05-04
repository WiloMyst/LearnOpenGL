#include "Renderer.h"

#include <iostream>

void GLClearError() //清除OpenGL错误
{
	while (glGetError() != GL_NO_ERROR); //循环清除错误
}

bool GLLogCall(const char* function, const char* file, int line) //检查OpenGL错误
{
	while (GLenum error = glGetError()) //循环获取错误
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function
			<< " " << file << ":" << line << std::endl; //输出错误信息
		return false; //返回false表示有错误
	}
	return true; //返回true表示没有错误
}