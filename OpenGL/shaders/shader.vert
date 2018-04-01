#version 330 core
  
layout(location=0) in vec3 vVertex;		//object space vertex position

void main()
{ 
	gl_Position = vec4(vVertex, 1.0f);
}