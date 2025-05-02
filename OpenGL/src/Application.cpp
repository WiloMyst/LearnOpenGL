#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); //创建着色器对象
	const char* src = source.c_str(); //获取着色器源代码
	glShaderSource(id, 1, &src, nullptr); //设置着色器源代码
	glCompileShader(id); //编译着色器

	int result; //编译结果
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //获取编译状态
	if (result == GL_FALSE) //编译失败
	{
		int length; //错误信息长度
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //获取错误信息长度
		char* message = (char*)alloca(length * sizeof(char)); //分配内存
		glGetShaderInfoLog(id, length, &length, message); //获取错误信息
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl; //输出错误信息
		std::cout << message << std::endl; //输出错误信息
		glDeleteShader(id); //删除着色器对象
		return 0; //返回0表示编译失败
	}

	return id; //返回着色器对象的ID
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //创建程序对象
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //编译顶点着色器
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //编译片段着色器

	glAttachShader(program, vs); //将顶点着色器附加到程序对象
	glAttachShader(program, fs); //将片段着色器附加到程序对象
	glLinkProgram(program); //链接程序对象
	glValidateProgram(program); //验证程序对象

	glDeleteShader(vs); //删除顶点着色器对象
	glDeleteShader(fs); //删除片段着色器对象
	return program; //返回程序对象的ID
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
	glfwMakeContextCurrent(window); //创建OpenGL上下文

	if (glewInit() != GLEW_OK) //检查GLEW是否初始化成功（需要在上下文创建后）
		std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = { //顶点位置数据
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

	unsigned int buffer; //缓冲区标识
	glGenBuffers(1, &buffer); //生成缓冲区对象
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //绑定缓冲区对象
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //将数据复制到缓冲区对象中
	// 指定缓冲区的内存布局
    glEnableVertexAttribArray(0); //启用通用顶点属性数组
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //定义通用顶点属性数据

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
	unsigned int shader = CreateShader(vertexShader, fragmentShader); //创建着色器程序
	glUseProgram(shader); //使用着色器程序

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //渲染循环
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //绘制三角形

		//glBegin(GL_TRIANGLES); //旧版OpenGL
		//glVertex2f(-0.5f, -0.5f);
        //glVertex2f( 0.0f,  0.5f);
		//glVertex2f( 0.5f, -0.5f);
		//glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteProgram(shader); //删除着色器程序

    glfwTerminate();
    return 0;
}