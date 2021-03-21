#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <fstream>

#include "stb_image.h"

float windowWidth = 1280;
float windowHeight = 720;

glm::vec3 lightSourcePos(1.2f, 1.0f, 1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = windowWidth / 2.0;
float lastY = windowHeight / 2.0;
float fov = 45.0f;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 2.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.07f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

void error(int error, const char* description)
{
    fputs(description, stderr);
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static ShaderProgramSource loadShaderFromFile(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

int main()
{
    glfwSetErrorCallback(error);

    if (!glfwInit())
        exit(1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "GLFW Test", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window); //GLFW context

    glfwSwapInterval(0); //vsync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK)
    {
        printf("Error: glew initialise error!\n");
        glfwTerminate();
        exit(1);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glewExperimental = GL_TRUE;
    glGetError();

    float vertices[] = {
            //     Coords            Tex coords         Normal cords
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,     0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,     0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,     1.0f, 1.0f,     0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
    };

    unsigned int vaoHandle;

    unsigned int vboHandles[1];
    glGenBuffers(1, vboHandles);
    unsigned int vertexBuffer = vboHandles[0];

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    ShaderProgramSource source = loadShaderFromFile("../basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    unsigned int lightPosLoc = glGetUniformLocation(shader, "lightPos");
    glUniform3f(lightPosLoc, lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);

    unsigned int objectColorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);

    unsigned int lightColorLoc = glGetUniformLocation(shader, "lightColor");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    unsigned char *data = stbi_load("../Textures/texture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);

    ShaderProgramSource lightSource = loadShaderFromFile("../lightSource.shader");
    unsigned int lightShader = CreateShader(lightSource.VertexSource, lightSource.FragmentSource);
    glUseProgram(lightShader);



    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(lightShader);

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightSourcePos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        glm::mat4 lightView = glm::mat4(1.0f);
        lightView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        unsigned int lightModelLoc = glGetUniformLocation(lightShader, "model");
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));

        unsigned int lightProjLocation = glGetUniformLocation(lightShader, "projection");
        glUniformMatrix4fv(lightProjLocation, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int lightViewLoc = glGetUniformLocation(lightShader, "view");
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(lightView));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        unsigned int modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int projLocation = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}