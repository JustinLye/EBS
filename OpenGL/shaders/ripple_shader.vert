#version 330 core
  
layout(location=0) in vec3 vVertex;		//object space vertex position

//uniforms


void main()
{ 
	//get the Euclidean distance of the current vertex from the center of the mesh
	float distance = length(vVertex);  
	//create a sin function using the distance, multiply frequency and add the elapsed time
	float y = amplitude*sin(-PI*distance*frequency+time);		
	//multiply the MVP matrix with the new position to get the clipspace position
	gl_Position = vec4(vVertex.x, y, vVertex.z,1);
}