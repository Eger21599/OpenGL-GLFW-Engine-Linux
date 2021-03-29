//
// Created by eger on 23.03.2021.
//

#ifndef GLFWOPENGL_INTERACTIONWITHSHADER_H
#define GLFWOPENGL_INTERACTIONWITHSHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class InteractionWithShader {
public:
    static int numberOfSpotLights;
    static int numberOfPointLights;

    void setVec3(unsigned int shaderProgram, char* name, glm::vec3 number);
    void setFloat(unsigned int shaderProgram, char* name, float number);

    void createSpotLight(unsigned int shaderProgram);

    void setMaterialWithMaps(unsigned int shaderProgram, glm::vec3 ambient, int diffuseIndex, int specularIndex, float shininess);

    void setMaterial(unsigned int shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

    void setDirLightIntensity(unsigned int shaderProgram, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void setPointLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                              float constant, float linear, float quadratic);

    void setSpotLightIntensity(unsigned int shaderProgram, int index, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant,  float linear, float quadratic,
                               glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};


#endif //GLFWOPENGL_INTERACTIONWITHSHADER_H
