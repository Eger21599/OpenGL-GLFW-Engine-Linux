  #shader vertex
  #version 460

  layout(location=0) in vec3 VertexPosition;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main()
  {
      gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  out vec4 FragColor;

  void main()
  {
      FragColor = vec4(1.0f);
  }