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

  uniform vec3 lightPos;
  uniform vec3 objectColor;
  uniform vec3 lightColor;

  uniform sampler2D Texture;

  out vec4 FragColor;

  void main()
  {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = /*texture(Texture, TexCoord) */ vec4(result, 1.0f);
  }