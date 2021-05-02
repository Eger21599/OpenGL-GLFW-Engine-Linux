#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
bool isPointLight = true;
struct Transform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale    = glm::vec3(0.5f, 0.5f, 0.5f);
};
float fogMaxDist = 50.0f;
float fogMinDist = 10.0f;
glm::vec3 fogColor = glm::vec3(0.475f, 0.475f, 0.475f);
float reflectFactor = 1.0f;
bool isFog = false;
bool isNormal = true;
bool isSkyBox = true;
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
unsigned int loadCubemap(std::vector<std::string> faces);

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

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    if (glewInit() != GLEW_OK)
    {
        printf("Error: glew initialise error!\n");
        glfwTerminate();
        exit(1);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glewExperimental = GL_TRUE;
    //stbi_set_flip_vertically_on_load(true);
    glGetError();

    float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
    };

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    Shader::ShaderProgramSource cubeShaderSource = Shader::loadShaderFromFile("../Shaders/cube.shader");
    unsigned int cubeShader = Shader::CreateShader(cubeShaderSource.VertexSource, cubeShaderSource.FragmentSource);
    glUseProgram(cubeShader);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    Shader::ShaderProgramSource skyboxShaderSource = Shader::loadShaderFromFile("../Shaders/skybox.shader");
    unsigned int skyboxShader = Shader::CreateShader(skyboxShaderSource.VertexSource, skyboxShaderSource.FragmentSource);
    glUseProgram(skyboxShader);

    glUseProgram(skyboxShader);

    Shader::setInt(skyboxShader, "skybox", 0);

    std::vector<std::string> faces
            {
                    "../Textures/SkyBoxes/sea/right.jpg",
                    "../Textures/SkyBoxes/sea/left.jpg",
                    "../Textures/SkyBoxes/sea/top.jpg",
                    "../Textures/SkyBoxes/sea/bottom.jpg",
                    "../Textures/SkyBoxes/sea/front.jpg",
                    "../Textures/SkyBoxes/sea/back.jpg"
            };
    unsigned int skyBoxCubemapTexture = loadCubemap(faces);

    std::vector<std::string> skyBoxFaces
            {
                    "../Textures/SkyBoxes/sea/right.jpg",
                    "../Textures/SkyBoxes/sea/left.jpg",
                    "../Textures/SkyBoxes/sea/top.jpg",
                    "../Textures/SkyBoxes/sea/bottom.jpg",
                    "../Textures/SkyBoxes/sea/front.jpg",
                    "../Textures/SkyBoxes/sea/back.jpg"
            };
    unsigned int cubeCubemapTexture = loadCubemap(skyBoxFaces);

    Model backpack("../Models/Backpack/backpack.obj", true);

    Shader::ShaderProgramSource basicShaderSource = Shader::loadShaderFromFile("../Shaders/basic.shader");
    unsigned int basicShader = Shader::CreateShader(basicShaderSource.VertexSource, basicShaderSource.FragmentSource);
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

        Shader::setMat4(basicShader, "model", model);
        Shader::setMat4(basicShader, "projection", projection);
        Shader::setMat4(basicShader, "view", view);

        Shader::setVec3(basicShader, "viewPos", cameraPos);

        Shader::setInt(basicShader, "numberOfSpotLights", Shader::numberOfSpotLights);
        Shader::setInt(basicShader, "numberOfPointLights", Shader::numberOfPointLights);

        Shader::setInt(basicShader, "fog.isFog", isFog);

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
        Shader::setMaterial(basicShader, shininess, isNormal);

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

        glUseProgram(cubeShader);

        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(2.6f, 0.0f, 0.0f));

        Shader::setMat4(cubeShader, "model", cubeModel);
        Shader::setMat4(cubeShader, "projection", projection);
        Shader::setMat4(cubeShader, "view", view);
        Shader::setVec3(cubeShader, "cameraPos", cameraPos);
        Shader::setFloat(cubeShader, "reflectFactor", reflectFactor);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //backpack.Draw(cubeShader);

        if(isSkyBox)
        {
            glDepthFunc(GL_LEQUAL);
            glUseProgram(skyboxShader);
            glm::mat4 skyboxView = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
            Shader::setMat4(skyboxShader, "view", skyboxView);
            Shader::setMat4(skyboxShader, "projection", projection);

            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

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

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void ModelGui()
{
    ImGui::Begin("Debug");
    ImGui::InputFloat("FOV", &fov);

    ImGui::Text("Transform");
    ImGui::InputFloat3("Position", &transform.position.x);
    ImGui::InputFloat3("Scale", &transform.scale.x);

    ImGui::Text("Cube");
    ImGui::SliderFloat("Reflection factor", &reflectFactor, 0.0f, 1.0f);

    ImGui::Text("Light settings");
    ImGui::Checkbox("Spot Light", &isSpotLight);
    ImGui::Checkbox("Point Light", &isPointLight);
    ImGui::InputFloat("Shininess", &shininess);
    ImGui::Checkbox("isNormal", &isNormal);

    ImGui::Text("Fog settings");
    ImGui::Checkbox("Fog", &isFog);
    ImGui::SliderFloat("Max Dist", &fogMaxDist, 0.0f, 100.0f);
    ImGui::SliderFloat("Min Dist", &fogMinDist, 0.0f, 100.0f);
    ImGui::ColorEdit3("Color", &fogColor.x);

    ImGui::Text("Skybox settings");
    ImGui::Checkbox("Skybox", &isSkyBox);

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}