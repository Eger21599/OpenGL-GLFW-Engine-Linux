//
// Created by eger on 23.03.2021.
//

#ifndef GLFWOPENGL_TEXTURE_H
#define GLFWOPENGL_TEXTURE_H

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
public:
    enum class types
    {
        PNG,
        JPG
    };

    unsigned int LoadTexture(const char* filePath, types typeOfFile);

private:
    unsigned int texture;
};


#endif //GLFWOPENGL_TEXTURE_H
