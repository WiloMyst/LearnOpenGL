#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
	std::string VertexSource; //������ɫ��Դ����
	std::string FragmentSource; //Ƭ����ɫ��Դ����
};

static ShaderProgramSource ParseShader(const std::string& filepath) //������ɫ��Դ����
{
	std::ifstream stream(filepath); //���ļ���

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	}; //��ɫ������
	ShaderType type = ShaderType::NONE; //��ʼ������ΪNONE

	std::string line; //�л�����
	std::stringstream ss[2]; //�ַ���������
	while (getline(stream, line)) //���ж�ȡ�ļ�
	{
		if (line.find("#shader") != std::string::npos) //����#shader�ؼ���
		{
			if (line.find("vertex") != std::string::npos) //����vertex�ؼ���
				type = ShaderType::VERTEX; //��������ΪVERTEX
			else if (line.find("fragment") != std::string::npos) //����fragment�ؼ���
				type = ShaderType::FRAGMENT; //��������ΪFRAGMENT
		}
		else if (type != ShaderType::NONE) //������Ͳ�ΪNONE
			ss[(int)type] << line << "\n"; //����д���Ӧ���ַ�������
	}
	return { ss[0].str(), ss[1].str() }; //���ض����Ƭ����ɫ��Դ����
}

static unsigned int CompileShader(unsigned int type, const std::string& source) //������ɫ��
{
	unsigned int id = glCreateShader(type); //������ɫ������
	const char* src = source.c_str(); //��ȡ��ɫ��Դ����
	glShaderSource(id, 1, &src, nullptr); //������ɫ��Դ����
	glCompileShader(id); //������ɫ��

	int result; //������
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //��ȡ����״̬
	if (result == GL_FALSE) //����ʧ��
	{
		int length; //������Ϣ����
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //��ȡ������Ϣ����
		char* message = (char*)alloca(length * sizeof(char)); //�����ڴ�
		glGetShaderInfoLog(id, length, &length, message); //��ȡ������Ϣ
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl; //���������Ϣ
		std::cout << message << std::endl; //���������Ϣ
		glDeleteShader(id); //ɾ����ɫ������
		return 0; //����0��ʾ����ʧ��
	}

	return id; //������ɫ�������ID
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //�����������
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //���붥����ɫ��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //����Ƭ����ɫ��

	glAttachShader(program, vs); //��������ɫ�����ӵ��������
	glAttachShader(program, fs); //��Ƭ����ɫ�����ӵ��������
	glLinkProgram(program); //���ӳ������
	glValidateProgram(program); //��֤�������

	glDeleteShader(vs); //ɾ��������ɫ������
	glDeleteShader(fs); //ɾ��Ƭ����ɫ������
	return program; //���س�������ID
}

int main(void)
{
	GLFWwindow* window; //����ָ��

    /* Initialize the library */
	if (!glfwInit()) //��ʼ��GLFW
        return -1;

    /* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); //��������
    if (!window)
    {
		glfwTerminate(); //�����������ʧ�ܣ���ֹGLFW
        return -1;
    }

    /* Make the window's context current */
	glfwMakeContextCurrent(window); //���õ�ǰ������

	glfwSwapInterval(1); //���ý������Ϊ1��������ֱͬ��

	if (glewInit() != GLEW_OK) //���GLEW�Ƿ��ʼ���ɹ�����Ҫ�������Ĵ�����
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl; //���OpenGL�汾
	{
		float positions[] = { //����λ������
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		unsigned int indices[] = { //��������
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao; //������������ʶ
		GLCall(glGenVertexArrays(1, &vao)); //���ɶ����������
		GLCall(glBindVertexArray(vao)); //�󶨶����������

		VertexBuffer vb(positions, 4 * 2 * sizeof(float)); //�������㻺��������

		// ָ���������Բ���
		GLCall(glEnableVertexAttribArray(0)); //���ö�����������(��������layout(location = 0))
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); //���嶥����������(��������0))�붥�㻺�����Ĺ�ϵ(��)

		IndexBuffer ib(indices, 6); //������������������

		ShaderProgramSource source = ParseShader("res/Shaders/Basic.shader"); //������ɫ��Դ����
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //������ɫ������
		GLCall(glUseProgram(shader)); //ʹ����ɫ������

		GLCall(int location = glGetUniformLocation(shader, "u_Color")); //��ȡͳһ����λ��
		ASSERT(location != -1); //���ͳһ����λ���Ƿ���Ч
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); //����ͳһ����ֵ

		float r = 0.0f; //��ɫ����
		float increment = 0.05f; //����

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) //��Ⱦѭ��
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT)); //�����ɫ������

			GLCall(glUseProgram(shader)); //ʹ����ɫ������
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); //����ͳһ����ֵ

			GLCall(glBindVertexArray(vao)); //�󶨶����������
			ib.Bind(); //������������

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //����ͼ��
			//glDrawArrays(GL_TRIANGLES, 0, 3); //����ͼ��

			if (r > 1.0f) //�����ɫ��������1.0
				increment = -0.05f; //��������Ϊ��
			else if (r < 0.0f) //�����ɫ����С��0.0
				increment = 0.05f; //��������Ϊ��
			r += increment; //���º�ɫ����

			/* Swap front and back buffers */
			glfwSwapBuffers(window); //����ǰ�󻺳���

			/* Poll for and process events */
			glfwPollEvents(); //�����¼�
		}

		glDeleteProgram(shader); //ɾ����ɫ������
	}
	glfwTerminate(); //��ֹGLFW
    return 0;
}