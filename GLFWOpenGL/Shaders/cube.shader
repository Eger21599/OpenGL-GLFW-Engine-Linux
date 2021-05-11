#shader vertex
#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 460

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform float reflectFactor;
uniform float ratio;

void main()
{
    float eta = 1.0f / ratio;
    vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    vec3 R = refract(I, normalize(Normal), eta);
    vec4 reflectedTexture = texture(skybox, vec3(-1.0f, 1.0f, -1.0f) * R);
    FragColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), reflectedTexture, reflectFactor);
}