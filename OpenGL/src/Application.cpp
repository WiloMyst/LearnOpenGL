#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak() //断言宏定义
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) //OpenGL调用宏定义

static void GLClearError() //清除OpenGL错误
{
	while (glGetError() != GL_NO_ERROR); //循环清除错误
}

static bool GLLogCall(const char* function, const char* file, int line) //检查OpenGL错误
{
	while (GLenum error = glGetError()) //循环获取错误
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function
			<< " " << file << ":" << line << std::endl; //输出错误信息
		return false; //返回false表示有错误
	}
	return true; //返回true表示没有错误
}

struct ShaderProgramSource
{
	std::string VertexSource; //顶点着色器源代码
	std::string FragmentSource; //片段着色器源代码
};

static ShaderProgramSource ParseShader(const std::string& filepath) //解析着色器源代码
{
	std::ifstream stream(filepath); //打开文件流

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	}; //着色器类型
	ShaderType type = ShaderType::NONE; //初始化类型为NONE

	std::string line; //行缓冲区
	std::stringstream ss[2]; //字符串流数组
	while (getline(stream, line)) //逐行读取文件
	{
		if (line.find("#shader") != std::string::npos) //查找#shader关键字
		{
			if (line.find("vertex") != std::string::npos) //查找vertex关键字
				type = ShaderType::VERTEX; //设置类型为VERTEX
			else if (line.find("fragment") != std::string::npos) //查找fragment关键字
				type = ShaderType::FRAGMENT; //设置类型为FRAGMENT
		}
		else if (type != ShaderType::NONE) //如果类型不为NONE
			ss[(int)type] << line << "\n"; //将行写入对应的字符串流中
	}
	return { ss[0].str(), ss[1].str() }; //返回顶点和片段着色器源代码
}

static unsigned int CompileShader(unsigned int type, const std::string& source) //编译着色器
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
	GLFWwindow* window; //窗口指针

    /* Initialize the library */
	if (!glfwInit()) //初始化GLFW
        return -1;

    /* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); //创建窗口
    if (!window)
    {
		glfwTerminate(); //如果创建窗口失败，终止GLFW
        return -1;
    }

    /* Make the window's context current */
	glfwMakeContextCurrent(window); //设置当前上下文

	glfwSwapInterval(1); //设置交换间隔为1，开启垂直同步

	if (glewInit() != GLEW_OK) //检查GLEW是否初始化成功（需要在上下文创建后）
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl; //输出OpenGL版本

    float positions[] = { //顶点位置数据
        -0.5f, -0.5f,
         0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
    };

	unsigned int indices[] = { //索引数据
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer; //缓冲区标识
	glGenBuffers(1, &buffer); //生成缓冲区对象
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //绑定缓冲区对象
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //将数据复制到缓冲区对象中
	// 指定缓冲区的内存布局
    glEnableVertexAttribArray(0); //启用通用顶点属性数组
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //定义通用顶点属性数据

	unsigned int ibo; //索引缓冲区标识
	glGenBuffers(1, &ibo); //生成索引缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //绑定索引缓冲区对象
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //将索引数据复制到索引缓冲区对象中

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); //解析着色器源代码
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //创建着色器程序
	glUseProgram(shader); //使用着色器程序

	int location = glGetUniformLocation(shader, "u_Color"); //获取统一变量位置
	ASSERT(location != -1); //检查统一变量位置是否有效
	glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f); //设置统一变量值

	float r = 0.0f; //红色分量
	float increment = 0.05f; //增量

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //渲染循环
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓冲区

		glUniform4f(location, r, 0.3f, 0.8f, 1.0f); //设置统一变量值
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //绘制图形
		//glDrawArrays(GL_TRIANGLES, 0, 3); //绘制图形

		if (r > 1.0f) //如果红色分量大于1.0
			increment = -0.05f; //设置增量为负
		else if (r < 0.0f) //如果红色分量小于0.0
			increment = 0.05f; //设置增量为正
		r += increment; //更新红色分量

        /* Swap front and back buffers */
		glfwSwapBuffers(window); //交换前后缓冲区

        /* Poll for and process events */
		glfwPollEvents(); //处理事件
    }

	glDeleteProgram(shader); //删除着色器程序

	glfwTerminate(); //终止GLFW
    return 0;
}