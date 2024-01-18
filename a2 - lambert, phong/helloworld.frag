/*Hello fragment shader!*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/* Passed time from the begining of the program */ 
uniform float iTime;

/*input variables*/
in vec4 vtx_color;
////TODO: define your own data channel(s) to receive the normal attribute from the vertex shader
in vec3 vtx_normal;
in float distanceToCam;

/*output variables*/
out vec4 frag_color;	/*or use gl_FragColor*/

void main()							
{	
    vec3 color = vec3(0.0, 0.0, 0.0);

    float pct = sin(iTime); //time variable
	////This is the default implementation of the fragment color. Change it to the color depending on the input normal value.
	vec3 norm = normalize(vtx_normal.xyz);

	vec3 diff = (1.f, 1.f, 1.f)-norm; // flip the norm

	color = mix(diff*(1/distanceToCam), norm, pct); // transition between colors over time
	
	frag_color = vec4(color, 1.f);
}	