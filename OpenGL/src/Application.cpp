#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

    float position[6] = { //顶点位置数据
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

	unsigned int Buffer; //缓冲区标识
	glGenBuffers(1, &Buffer); //生成缓冲区对象
	glBindBuffer(GL_ARRAY_BUFFER, Buffer); //绑定缓冲区对象
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW); //将数据复制到缓冲区对象中

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //绘制三角形

		//glBegin(GL_TRIANGLES);
		//glVertex2f(-0.5f, -0.5f);
        //glVertex2f( 0.0f,  0.5f);
		//glVertex2f( 0.5f, -0.5f);
		//glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}