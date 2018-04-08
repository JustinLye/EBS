#version 330 core
layout (triangles) in;
uniform int maxv;
layout (triangle_strip, max_vertices=256) out; 
 
//uniforms
uniform int sub_divisions;		 //the number of subdivisions
uniform mat4 MVP;				 //combined view porjection matrix
uniform float time;		//elapsed time 
uniform float amplitude;
uniform float frequency;
//shader constants

const float PI = 3.14159;

float SinY(vec3,vec3);

void main()
{
	//get the object space vertex positions
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position; 

	//determine the size of each sub-division 
	float dx = abs(v0.x-v2.x)/sub_divisions;
	float dz = abs(v0.z-v1.z)/sub_divisions;

	float x=v0.x;
	float z=v0.z;
	float y = 0;

	//loop through all sub-divisions and emit vertices
	//after mutiplying the object space vertex positions
	//with the combined modelview projection matrix. We 
	//move first in x axis, once we reach the edge, we 
	//reset x to the initial x value while incrementing 
	//the z value.
	
	for(int j=0;j<sub_divisions*sub_divisions;j++) {
		y = SinY(vec3(x,0,z),vec3(0,0,0));
		gl_Position =  MVP*vec4(x,y,z,1);       EmitVertex();		
		y = SinY(vec3(x,y,z+dz),vec3(0));
		gl_Position =  MVP * vec4(x,-1*y,z+dz,1);     EmitVertex();				  
		y = SinY(vec3(x+dx, y, z),vec3(0));
		gl_Position =  MVP * vec4(x+dx,y,z,1);     EmitVertex(); 
		y = SinY(vec3(x+dx, y, z+dz),vec3(0));
		gl_Position =  MVP * vec4(x+dx,-1*y,z+dz,1);  EmitVertex();
		EndPrimitive();	 
		x+=dx;

		if((j+1) %sub_divisions == 0) {
		   x=v0.x;
		   z+=dz;
		   y=0;
		}	
	}	
}

float SinY(vec3 v, vec3 orig)
{
	float d = distance(v,orig);
	float y = amplitude*sin(-PI*d*frequency+time);
	return y;
}