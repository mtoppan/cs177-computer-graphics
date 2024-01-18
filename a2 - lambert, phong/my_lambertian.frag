/*This is your first fragment shader!*/

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
////TODO [Step 2]: add your in variables from the vertex shader
in vec3 nml;
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;

vec3 lightPosArray[5] = vec3[5] (vec3(-200, 500, 300), vec3(30, 10, -30), vec3(0, 400, 0), vec3(0, 10, 0), vec3(10, 5, 15));
////TODO [Step 2]: add your Lambertian lighting parameters here
vec3 Id[5] = vec3[5] (vec3(0.9f, 0.8f, 1.f), vec3(0.8f, 0.9f, 0.7f), vec3(0.3f, 0.3f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f));
vec3 Ia[5] = vec3[5] (vec3(0.6f, 0.6f, 0.6f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f));
const float ka = 0.05;
const float kd = 0.8f;

void main()							
{	
	vec3 amb_color = vec3(0.f, 0.f, 0.f);
	vec3 diff_color = vec3(0.f, 0.f, 0.f);

	for (int i=0; i<lightPosArray.length(); i++){ //loop throug all lights in scene
		amb_color += ka*Ia[i];

		vec3 l = normalize((lightPosArray[i]-vtx_pos));
		
		float l_dot_n = dot(l, nml);
		float diff = max(0.f, l_dot_n);
		diff_color += kd*diff*Id[i]*(vtx_color.rgb);
	}

	vec4 diff_final = vec4(diff_color, 1.f); // sum diffuse
	
	vec4 amb_final = vec4(amb_color, 1.f); //sum ambient

	frag_color = amb_final + diff_final; //sum them all
}	