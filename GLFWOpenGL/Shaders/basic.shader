#shader vertex
#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 Position;

out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  TexCoords = aTexCoords;
  Normal = mat3(transpose(inverse(model))) * aNormal;
  FragPos = vec3(model * vec4(aPos, 1.0f));
  Position = vec3(view * vec4(aPos, 1.0f));

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);

  TBN = transpose(mat3(T, B, N));

  gl_Position = projection * view * model * vec4(aPos, 1.0);
}

  #shader fragment
  #version 460

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 Position;

in mat3 TBN;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  sampler2D texture_normal1;

  float shininess;
  bool isNormal;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct FogInfo {
  bool isFog;

  float maxDist;
  float minDist;

  vec3 color;
};
#define NR_POINT_LIGHTS 10
#define NR_SPOT_LIGHTS 10

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform FogInfo fog;

uniform int numberOfPointLights;
uniform int numberOfSpotLights;
uniform vec3 viewPos;

vec3 TangentViewPos;
vec3 TangentFragPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  vec3 TangentViewPos  = TBN * viewPos;
  vec3 TangentFragPos  = TBN * FragPos;

  vec3 viewDir = vec3(0.0f);
  vec3 norm = vec3(0.0f);

  if(material.isNormal)
  {
    viewDir = normalize(TangentViewPos - TangentFragPos);
    norm = texture(material.texture_normal1, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
  }
  else
  {
    viewDir = normalize(viewPos - FragPos);
    norm = normalize(Normal);
  }

  vec3 result = CalcDirLight(dirLight, norm, viewDir);

  //if(!gl_FrontFacing)
    //norm = -norm;

  if(numberOfPointLights > 0)
  {
    for (int i = 0; i < numberOfPointLights; i++)
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
  }

  if(numberOfSpotLights > 0)
  {
    for (int j = 0; j < numberOfSpotLights; j++)
    result += CalcSpotLight(spotLights[j], norm, FragPos, viewDir);
  }

  if(fog.isFog)
  {
    float dist = length(Position.xyz);
    float fogFactor = (fog.maxDist - dist) / (fog.maxDist - fog.minDist);
    fogFactor = clamp(fogFactor, 0.0f, 1.0f);

    result = mix(fog.color, result, fogFactor);
  }

  FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = vec3(0.0f);

  if(material.isNormal)
  {
    vec3 TangentLightPos = TBN * light.direction;
    lightDir = normalize(TangentLightPos - TangentFragPos);
  }
  else
    lightDir = normalize(light.direction - FragPos);

  float sDotN = max(dot(lightDir, normal), 0.0f);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);

  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = vec3(0.0f);
  if (sDotN > 0.0f)
  {
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
  }
  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = vec3(0.0f);
  vec3 TangentLightPos = TBN * light.position;

  if(material.isNormal)
  {
    lightDir = normalize(TangentLightPos - fragPos);
  }
  else
    lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}