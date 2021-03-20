  #shader vertex
  #version 460

  layout(location=0) in vec3 VertexPosition;
  layout(location=1) in vec3 VertexColor;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  out vec3 Color;

  void main()
  {
  	Color = VertexColor;
    gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  in vec3 Color;

  out vec4 FragColor;

  void main()
  {
  	FragColor = vec4(Color/*0.0f, 1.0f, 0.0f*/, 1.0f);
  }