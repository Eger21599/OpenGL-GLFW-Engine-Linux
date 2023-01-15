//
// Created by eger on 29.03.2021.
//

#include "Shader.h"

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>

void Shader::setVec3(unsigned int shaderProgram, std::string name, glm::vec3 value)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setFloat(unsigned int shaderProgram, std::string name, float value)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(location, value);
}

void Shader::setInt(unsigned  int shaderProgram, std::string name, int value)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(location, value);
}

void Shader::setMat4(unsigned int shaderProgram, std::string name, glm::mat4 value)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

Shader::ShaderProgramSource Shader::loadShaderFromFile(const std::string& filePath)
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

void Shader::setMaterialWithMaps(unsigned int shaderProgram, glm::vec3 ambient, int diffuseIndex, int specularIndex, float shininess)
{
    unsigned int ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    unsigned int diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    unsigned int specularLoc = glGetUniformLocation(shaderProgram, "material.specular");
    unsigned int shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");

    glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    glUniform1i(diffuseLoc, diffuseIndex);
    glUniform1i(specularLoc, specularIndex);
    glUniform1f(shininessLoc, shininess);
}

void Shader::setMaterial(unsigned int shaderProgram, float shininess, bool isNormal)
{
    unsigned int shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
    unsigned int isNormalLoc = glGetUniformLocation(shaderProgram, "material.isNormal");

    glUniform1f(shininessLoc, shininess);
    glUniform1i(isNormalLoc, isNormal);
}

void Shader::setDirLightIntensity(unsigned int shaderProgram, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    unsigned int directionLoc = glGetUniformLocation(shaderProgram, "dirLight.direction");
    unsigned int ambientLoc = glGetUniformLocation(shaderProgram, "dirLight.ambient");
    unsigned int diffuseLoc = glGetUniformLocation(shaderProgram, "dirLight.diffuse");
    unsigned int specularLoc = glGetUniformLocation(shaderProgram, "dirLight.specular");

    glUniform3f(directionLoc, direction.x, direction.y, direction.z);
    glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLoc, specular.x, specular.y, specular.z);
}

void Shader::setSpotLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant,
                                                  float linear, float quadratic,
                                                  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    std::string indexString = std::to_string(index);

    unsigned int positionLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].position").c_str());
    unsigned int directionLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].direction").c_str());
    unsigned int cutOffLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].cutOff").c_str());
    unsigned int outerCutOffLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].outerCutOff").c_str());
    unsigned int constantLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].constant").c_str());
    unsigned int linearLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].linear").c_str());
    unsigned int quadraticLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].quadratic").c_str());
    unsigned int ambientLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].ambient").c_str());
    unsigned int diffuseLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].diffuse").c_str());
    unsigned int specularLoc = glGetUniformLocation(shaderProgram, ("spotLights[" + indexString + "].specular").c_str());

    glUniform3f(positionLoc, position.x, position.y, position.z);
    glUniform3f(directionLoc, direction.x, direction.y, direction.z);
    glUniform1f(cutOffLoc, cutOff);
    glUniform1f(outerCutOffLoc, outerCutOff);
    glUniform1f(constantLoc, constant);
    glUniform1f(linearLoc, linear);
    glUniform1f(quadraticLoc, quadratic);
    glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLoc, specular.x, specular.y, specular.z);
}

void Shader::setPointLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position,
                                                   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                                   float constant, float linear, float quadratic)
{
    std::string indexString = std::to_string(index);

    unsigned int posLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].position").c_str());
    unsigned int ambientLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].ambient").c_str());
    unsigned int diffuseLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].diffuse").c_str());
    unsigned int specularLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].specular").c_str());
    unsigned int constantLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].constant").c_str());
    unsigned int linearLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].linear").c_str());
    unsigned int quadraticLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + indexString + "].quadratic").c_str());

    glUniform3f(posLoc, position.x, position.y, position.z);
    glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLoc, specular.x, specular.y, specular.z);
    glUniform1f(constantLoc, constant);
    glUniform1f(linearLoc, linear);
    glUniform1f(quadraticLoc, quadratic);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
