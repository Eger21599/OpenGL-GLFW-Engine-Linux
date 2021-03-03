  #shader vertex
  #version 460

  layout(location=0) in vec3 VertexPosition;
  layout(location=1) in vec3 VertexColor;

  uniform vec3 u_Color;

  out vec3 Color;

  void main()
  {
  	Color = VertexColor /*vec3(0.0f, 0.0f, 0.7f)*/ /*u_Color*/;
  	gl_Position = vec4(VertexPosition, 1.0f);
  }

  #shader fragment
  #version 460

  in vec3 Color;

  out vec4 FragColor;

  void main()
  {
  	FragColor = vec4(Color, 1.0f);
  }