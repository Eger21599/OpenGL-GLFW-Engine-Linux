//
// Created by eger on 29.03.2021.
//

#ifndef GLFWOPENGL_SHADER_H
#define GLFWOPENGL_SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    static int numberOfSpotLights;
    static int numberOfPointLights;

    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    static void setVec3(unsigned int shaderProgram, std::string name, glm::vec3 value);
    static void setFloat(unsigned int shaderProgram, std::string name, float value);

    static ShaderProgramSource loadShaderFromFile(const std::string& filePath);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    static void setMaterialWithMaps(unsigned int shaderProgram, glm::vec3 ambient, int diffuseIndex, int specularIndex, float shininess);

    static void setMaterial(unsigned int shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

    static void setDirLightIntensity(unsigned int shaderProgram, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    static void setPointLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                       float constant, float linear, float quadratic);

    static void setSpotLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant,  float linear, float quadratic,
                                      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

private:
    static unsigned int CompileShader(unsigned int type, const std::string& source);

};


#endif //GLFWOPENGL_SHADER_H
