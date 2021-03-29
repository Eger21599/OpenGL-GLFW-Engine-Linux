//
// Created by eger on 29.03.2021.
//

#ifndef GLFWOPENGL_MODEL_H
#define GLFWOPENGL_MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model {
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(char *path)
    {
        loadModel(path);
    }

    void Draw(unsigned int &shaderProgram);

private:
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};


#endif //GLFWOPENGL_MODEL_H
