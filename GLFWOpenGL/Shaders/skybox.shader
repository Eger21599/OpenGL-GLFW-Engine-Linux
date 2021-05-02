#shader vertex
#version 460
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vec3(aPos.x, aPos.y, aPos.z);

    vec4 pos = projection * view * vec4(aPos, 1.0f);

    gl_Position = pos.xyww;
}

#shader fragment
#version 460

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}