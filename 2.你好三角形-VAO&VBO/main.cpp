#include<glad/glad.h>
#include "glfw3.h"
#include<iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


/*
    GLuint VBO, VAO;
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
解释下这段代码
这段代码主要是用来创建并配置一个VAO和一个VBO，并将顶点数据存储到VBO中。

首先，使用 glGenBuffers() 函数生成一个VBO和VAO。然后，使用 glBindVertexArray() 函数绑定VAO，使用 glBindBuffer() 函数将VBO绑定到GL_ARRAY_BUFFER目标上。

接下来，使用 glBufferData() 函数将顶点数据存储到VBO中。这个函数的第一个参数是目标缓冲区类型，这里是GL_ARRAY_BUFFER。第二个参数是存储数据的大小，第三个参数是要存储的数据。第四个参数是数据如何使用，这里是GL_STATIC_DRAW，表示数据不会频繁修改，适合使用GPU的缓存。

然后，使用 glVertexAttribPointer() 函数告诉OpenGL如何解析顶点数据。第一个参数是顶点属性的位置，这里是0。第二个参数是顶点属性的大小，这里是3，表示每个顶点由3个浮点数组成。第三个参数是数据的类型，这里是GL_FLOAT。第四个参数是是否需要归一化，这里是GL_FALSE。第五个参数是步长，即每个顶点属性之间的间隔，这里是3 * sizeof(float)。最后一个参数是偏移量，即数据从哪里开始，这里是0。

接着，使用 glEnableVertexAttribArray() 函数启用顶点属性数组。最后，使用 glBindVertexArray() 和 glBindBuffer() 函数将VAO和VBO解绑，以避免意外修改它们的状态。

总之，这段代码的作用是将顶点数据存储到VBO中，并将其绑定到VAO中，以便OpenGL可以正确地解析和渲染这些顶点数据。


    glBindVertexArray(0)解绑后，VAO上的数据还存在吗
是的，调用 glBindVertexArray(0) 函数将VAO解绑后，VAO上的数据仍然存在。因为VAO本身只是一个记录状态的对象，它所记录的状态包括绑定的VBO、顶点属性指针等，而这些状态都是持久的，直到您显式地更改它们或删除VAO为止。

因此，在解绑VAO后，您仍然可以使用之前绑定的VBO和顶点属性指针来渲染模型。但是，为了代码的可读性和可维护性，建议在每次渲染之前都显式地绑定VAO，以确保代码的正确性。

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void generate_triangel_with_vertices_amount(float vertices[], size_t amount)
{
    if (amount % 3 != 0)
    {
        assert(false);
    }

    // 生成amount个随机数
    for (size_t i = 0; i < amount / 3; i++)
    {
        int start = i * 3;
        int flag = rand() > RAND_MAX / 2 ? -1 : 1;
        vertices[start] = rand() / double(RAND_MAX) * flag;
        vertices[start + 1] = rand() / double(RAND_MAX) * flag;
        vertices[start + 2] = 0.0f;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用兼容性模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 生成三角形顶点数据
    const int vertices_amount = 9;
    float vertices[vertices_amount];

    generate_triangel_with_vertices_amount(vertices, vertices_amount);

    // 开始使用VBO绑定
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 开始编译着色器
    unsigned int verShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(verShader, 1, &vertexShaderSource, NULL);
    glCompileShader(verShader);

    int result;
    char infoLog[512];
    glGetShaderiv(verShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(verShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    unsigned int framentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(framentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(framentShader);

    glGetShaderiv(framentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(framentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    // 到这里 编译完成, 开始链接
    unsigned int shaderProgramer = glCreateProgram();
    glAttachShader(shaderProgramer, verShader);
    glAttachShader(shaderProgramer, framentShader);
    glLinkProgram(shaderProgramer);

    glGetProgramiv(shaderProgramer, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgramer, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    // 清理下shader
    glDeleteShader(verShader);
    glDeleteShader(framentShader);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramer);
        // 绑定VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理操作
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgramer);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}