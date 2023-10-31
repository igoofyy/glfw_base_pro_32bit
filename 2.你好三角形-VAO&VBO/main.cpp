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
��������δ���
��δ�����Ҫ����������������һ��VAO��һ��VBO�������������ݴ洢��VBO�С�

���ȣ�ʹ�� glGenBuffers() ��������һ��VBO��VAO��Ȼ��ʹ�� glBindVertexArray() ������VAO��ʹ�� glBindBuffer() ������VBO�󶨵�GL_ARRAY_BUFFERĿ���ϡ�

��������ʹ�� glBufferData() �������������ݴ洢��VBO�С���������ĵ�һ��������Ŀ�껺�������ͣ�������GL_ARRAY_BUFFER���ڶ��������Ǵ洢���ݵĴ�С��������������Ҫ�洢�����ݡ����ĸ��������������ʹ�ã�������GL_STATIC_DRAW����ʾ���ݲ���Ƶ���޸ģ��ʺ�ʹ��GPU�Ļ��档

Ȼ��ʹ�� glVertexAttribPointer() ��������OpenGL��ν����������ݡ���һ�������Ƕ������Ե�λ�ã�������0���ڶ��������Ƕ������ԵĴ�С��������3����ʾÿ��������3����������ɡ����������������ݵ����ͣ�������GL_FLOAT�����ĸ��������Ƿ���Ҫ��һ����������GL_FALSE������������ǲ�������ÿ����������֮��ļ����������3 * sizeof(float)�����һ��������ƫ�����������ݴ����￪ʼ��������0��

���ţ�ʹ�� glEnableVertexAttribArray() �������ö����������顣���ʹ�� glBindVertexArray() �� glBindBuffer() ������VAO��VBO����Ա��������޸����ǵ�״̬��

��֮����δ���������ǽ��������ݴ洢��VBO�У�������󶨵�VAO�У��Ա�OpenGL������ȷ�ؽ�������Ⱦ��Щ�������ݡ�


    glBindVertexArray(0)����VAO�ϵ����ݻ�������
�ǵģ����� glBindVertexArray(0) ������VAO����VAO�ϵ�������Ȼ���ڡ���ΪVAO����ֻ��һ����¼״̬�Ķ���������¼��״̬�����󶨵�VBO����������ָ��ȣ�����Щ״̬���ǳ־õģ�ֱ������ʽ�ظ������ǻ�ɾ��VAOΪֹ��

��ˣ��ڽ��VAO������Ȼ����ʹ��֮ǰ�󶨵�VBO�Ͷ�������ָ������Ⱦģ�͡����ǣ�Ϊ�˴���Ŀɶ��ԺͿ�ά���ԣ�������ÿ����Ⱦ֮ǰ����ʽ�ذ�VAO����ȷ���������ȷ�ԡ�

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void generate_triangel_with_vertices_amount(float vertices[], size_t amount)
{
    if (amount % 3 != 0)
    {
        assert(false);
    }

    // ����amount�������
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
    //ʹ�ü�����ģʽ
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

    // ���������ζ�������
    const int vertices_amount = 9;
    float vertices[vertices_amount];

    generate_triangel_with_vertices_amount(vertices, vertices_amount);

    // ��ʼʹ��VBO��
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ��ʼ������ɫ��
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

    // ������ �������, ��ʼ����
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

    // ������shader
    glDeleteShader(verShader);
    glDeleteShader(framentShader);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramer);
        // ��VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // �������
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