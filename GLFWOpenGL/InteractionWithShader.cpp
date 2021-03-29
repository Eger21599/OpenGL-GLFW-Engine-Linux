//
// Created by eger on 23.03.2021.
//

#include <iostream>

#include "InteractionWithShader.h"

#include <GL/glew.h>

#include <glm/glm.hpp>

void InteractionWithShader::setVec3(unsigned int shaderProgram, char* name, glm::vec3 number)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform3f(location, number.x, number.y, number.z);
}

void InteractionWithShader::setFloat(unsigned int shaderProgram, char* name, float number)
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform1f(location, number);
}

void InteractionWithShader::setMaterialWithMaps(unsigned int shaderProgram, glm::vec3 ambient, int diffuseIndex, int specularIndex, float shininess)
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

void InteractionWithShader::setMaterial(unsigned int shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    unsigned int ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    unsigned int diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    unsigned int specularLoc = glGetUniformLocation(shaderProgram, "material.specular");
    unsigned int shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");

    glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLoc, specular.x, specular.y, specular.z);
    glUniform1f(shininessLoc, shininess);
}

void InteractionWithShader::setDirLightIntensity(unsigned int shaderProgram, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
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

void InteractionWithShader::setSpotLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant,
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

void InteractionWithShader::setPointLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position,
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
