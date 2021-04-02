  #shader vertex
  #version 460

  layout(location=0) in vec3 vertexPosition;
  layout(location=1) in vec2 texCoord;
  layout(location=2) in vec3 vertexNormal;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  out vec2 TexCoords;
  out vec3 Normal;
  out vec3 FragPos;

  void main()
  {
      TexCoords = texCoord;
      Normal = mat3(transpose(inverse(model))) * vertexNormal;
      FragPos = vec3(model * vec4(vertexPosition, 1.0f));

      gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  in vec2 TexCoords;
  in vec3 Normal;
  in vec3 FragPos;

  struct Material {
      sampler2D diffuse;
      sampler2D specular;
      float shininess;
  };

  struct DirLight {
      vec3 direction;

      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
  };

  struct PointLight {
      vec3 position;

      float constant;
      float linear;
      float quadratic;

      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
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

  #define NR_POINT_LIGHTS 10
  #define NR_SPOT_LIGHTS 10

  uniform int numberOfSpotLights;
  uniform int numberOfPointLights;
  uniform Material material;
  uniform SpotLight spotLights[NR_SPOT_LIGHTS];
  uniform DirLight dirLight;
  uniform PointLight pointLights[NR_POINT_LIGHTS];

  uniform vec3 viewPos;

  out vec4 FragColor;

  vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
  vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
  vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

  void main()
  {
      vec3 norm = normalize(Normal);
      vec3 viewDir = normalize(viewPos - FragPos);

      vec3 result = CalcDirLight(dirLight, norm, viewDir);

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

      FragColor = vec4(result, 1.0);
  }

  vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
  {
      vec3 lightDir = normalize(light.direction - FragPos);
      float sDotN = max(dot(lightDir, normal), 0.0f);

      float diff = max(dot(normal, lightDir), 0.0);

      vec3 reflectDir = reflect(-lightDir, normal);

      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = vec3(0.0f);
      if (sDotN > 0.0f)
      {
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
          specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      }
      return (ambient + diffuse + specular);
  }

  vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
  {
      vec3 lightDir = normalize(light.position - fragPos);

      float diff = max(dot(normal, lightDir), 0.0);

      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

      float distance = length(light.position - fragPos);
      float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
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

      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      ambient *= attenuation * intensity;
      diffuse *= attenuation * intensity;
      specular *= attenuation * intensity;
      return (ambient + diffuse + specular);
  }