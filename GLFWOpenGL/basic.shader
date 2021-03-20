  #shader vertex
  #version 460

  layout(location=0) in vec3 VertexPosition;
  layout(location=1) in vec2 texCoord;
  layout(location=2) in vec3 VertexColor;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  out vec3 Color;
  out vec2 TexCoord;

  void main()
  {
  	Color = VertexColor;
    TexCoord = texCoord;
    gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  in vec3 Color;
  in vec2 TexCoord;

  uniform sampler2D Texture;

  out vec4 FragColor;

  void main()
  {
  	FragColor = texture(Texture, TexCoord) /* vec4(Color, 1.0f)*/;
  }