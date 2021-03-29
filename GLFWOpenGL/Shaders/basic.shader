  #shader vertex
  #version 460

  layout(location=0) in vec3 vertexPosition;
  layout(location=1) in vec2 texCoord;
  layout(location=2) in vec3 vertexNormal;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  out vec2 TexCoord;
  out vec3 Normal;
  out vec3 FragPos;

  void main()
  {
    TexCoord = texCoord;
    Normal = mat3(transpose(inverse(model))) * vertexNormal;
    FragPos = vec3(model * vec4(vertexPosition, 1.0f));

    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  in vec2 TexCoord;
  in vec3 Normal;
  in vec3 FragPos;

  struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
  };

  struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  };

  uniform Material material;
  uniform Light light;

  uniform vec3 lightPos;
  uniform vec3 objectColor;
  uniform vec3 lightColor;
  uniform vec3 viewPos;

  uniform sampler2D Texture;

  out vec4 FragColor;

  void main()
  {
    vec3 ambient  = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = texture(Texture, TexCoord) * vec4(result, 1.0f);
  }