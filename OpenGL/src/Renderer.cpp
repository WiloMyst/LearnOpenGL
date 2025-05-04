#include "Renderer.h"

#include <iostream>

void GLClearError() //���OpenGL����
{
	while (glGetError() != GL_NO_ERROR); //ѭ���������
}

bool GLLogCall(const char* function, const char* file, int line) //���OpenGL����
{
	while (GLenum error = glGetError()) //ѭ����ȡ����
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function
			<< " " << file << ":" << line << std::endl; //���������Ϣ
		return false; //����false��ʾ�д���
	}
	return true; //����true��ʾû�д���
}