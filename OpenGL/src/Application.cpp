#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
	glfwMakeContextCurrent(window); //����OpenGL������

	if (glewInit() != GLEW_OK) //���GLEW�Ƿ��ʼ���ɹ�����Ҫ�������Ĵ�����
		std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = { //����λ������
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

	unsigned int buffer; //��������ʶ
	glGenBuffers(1, &buffer); //���ɻ���������
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //�󶨻���������
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //�����ݸ��Ƶ�������������
	// ָ�����������ڴ沼��
    glEnableVertexAttribArray(0); //����ͨ�ö�����������
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //����ͨ�ö�����������

	std::string vertexShader = 
		"#version 330 core\n"
		"layout(location = 0) in vec4 position;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = position;\n"
		"}\n";
	std::string fragmentShader =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"void main()\n"
		"{\n"
		"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int shader = CreateShader(vertexShader, fragmentShader); //������ɫ������
	glUseProgram(shader); //ʹ����ɫ������

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //��Ⱦѭ��
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //����������

		//glBegin(GL_TRIANGLES); //�ɰ�OpenGL
		//glVertex2f(-0.5f, -0.5f);
        //glVertex2f( 0.0f,  0.5f);
		//glVertex2f( 0.5f, -0.5f);
		//glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteProgram(shader); //ɾ����ɫ������

    glfwTerminate();
    return 0;
}