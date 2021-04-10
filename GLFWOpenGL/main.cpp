#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_opengl3.h"
#include "libs/imgui/imgui_impl_glfw.h"

#include "Header files/stb_image.h"
#include "Header files/Shader.h"
#include "Header files/Model.h"

float windowWidth = 1280;
float windowHeight = 720;

glm::vec3 lightSourcePos(1.2f, 1.0f, -3.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool isRightButton = false;

///////////////////////////// Model Gui ///////////////////////////////////////
float shininess = 32.0f;
bool isSpotLight = false;
bool isPointLight = false;
struct Transform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale    = glm::vec3(0.5f, 0.5f, 0.5f);
};
float fogMaxDist = 50.0f;
float fogMinDist = 10.0f;
glm::vec3 fogColor = glm::vec3(0.475f, 0.475f, 0.475f);
bool isFog = false;
///////////////////////////////////////////////////////////////////////////////

Transform transform;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = windowWidth / 2.0;
float lastY = windowHeight / 2.0;
float fov = 45.0f;

int Shader::numberOfSpotLights = 0;
int Shader::numberOfPointLights = 1;

void processInput(GLFWwindow*);
void mouse_callback(GLFWwindow*, double, double);
void ModelGui();

void error(int error, const char* description)
{
    fputs(description, stderr);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    glfwSwapInterval(0); //vsync
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if (glewInit() != GLEW_OK)
    {
        printf("Error: glew initialise error!\n");
        glfwTerminate();
        exit(1);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glewExperimental = GL_TRUE;
    stbi_set_flip_vertically_on_load(true);
    glGetError();

    Model backpack("../Models/Backpack/backpack.obj");

    Shader::ShaderProgramSource source = Shader::loadShaderFromFile("../Shaders/basic.shader");
    unsigned int basicShader = Shader::CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(basicShader);

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

        if(isFog)
            glClearColor(0.475f, 0.475f, 0.475f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(isRightButton)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        lightSourcePos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightSourcePos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        //lightSourcePos.z = glfwGetTime() / 2.0f;

        glUseProgram(basicShader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, transform.scale);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(basicShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int projLocation = glGetUniformLocation(basicShader, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(basicShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        Shader::setVec3(basicShader, "viewPos", cameraPos);

        unsigned int numberOfSpotLightsLocation = glGetUniformLocation(basicShader, "numberOfSpotLights");
        glUniform1i(numberOfSpotLightsLocation, Shader::numberOfSpotLights);

        unsigned int numberOfPointLightsLocation = glGetUniformLocation(basicShader, "numberOfPointLights");
        glUniform1i(numberOfPointLightsLocation, Shader::numberOfPointLights);

        /*int fogOn = 0;
        if(isFog)
            fogOn = 1;
        else
            fogOn = 0;*/

        unsigned int isFogLoc = glGetUniformLocation(basicShader, "fog.isFog");
        glUniform1i(isFogLoc, isFog);

        Shader::setFloat(basicShader, "fog.maxDist", fogMaxDist);
        Shader::setFloat(basicShader, "fog.minDist", fogMinDist);
        Shader::setVec3(basicShader, "fog.color", fogColor);

        if(isSpotLight)
            Shader::numberOfSpotLights = 1;
        else
            Shader::numberOfSpotLights = 0;

        if(isPointLight)
            Shader::numberOfPointLights = 1;
        else
            Shader::numberOfPointLights = 0;

///////////////////////////////////////////////////////////// LIGHT AND MATERIAL SETTINGS //////////////////////////////////////////////////////////////////////////////////////////////////////////
        Shader::setMaterial(basicShader, shininess);

        Shader::setDirLightIntensity(basicShader, lightSourcePos, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f),
                                                 glm::vec3(1.0f, 1.0f, 1.0f));

        Shader::setSpotLightIntensity(basicShader, 0, cameraPos, cameraFront,
                                                   glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)),
                                                   1.0f, 0.22f, 0.20f,
                                                   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                                   glm::vec3(1.0f, 1.0f, 1.0f));

        Shader::setPointLightIntensity(basicShader, 0, glm::vec3(0.7f,  0.2f,  2.0f), glm::vec3(0.05f, 0.05f, 0.05f),
                                                     glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
                                                     1.0f, 0.09f, 0.032f);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        backpack.Draw(basicShader);

        ModelGui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteProgram(basicShader);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        isRightButton = true;
    else
        isRightButton = false;

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

    if(isRightButton)
    {
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
}

void ModelGui()
{
    ImGui::Begin("Debug");
    ImGui::InputFloat("FOV", &fov);

    ImGui::Text("Transform");
    ImGui::InputFloat3("Position", &transform.position.x);
    ImGui::InputFloat3("Scale", &transform.scale.x);

    ImGui::Text("Light settings");
    ImGui::Checkbox("Spot Light", &isSpotLight);
    ImGui::Checkbox("Point Light", &isPointLight);
    ImGui::InputFloat("Shininess", &shininess);

    ImGui::Text("Fog settings");
    ImGui::Checkbox("Fog", &isFog);
    ImGui::SliderFloat("Max Dist", &fogMaxDist, 0.0f, 100.0f);
    ImGui::SliderFloat("Min Dist", &fogMinDist, 0.0f, 100.0f);
    ImGui::ColorEdit3("Color", &fogColor.x);

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}