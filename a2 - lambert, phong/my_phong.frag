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
in vec3 nml;
in vec3 vtx_pos;
////TODO [Step 2]: add your in variables from the vertex shader

/*output variables*/
out vec4 frag_color;

/*hard-coded lighting parameters*/

////TODO [Step 3]: add your Phong lighting parameters here
float shiny = 32.f;

vec3 lightPosArray[5] = vec3[5] (vec3(-200, 500, 300), vec3(250, 100, 300), vec3(0, 500, 0), vec3(0, 3, 0), vec3(5, 3, 5));
////TODO [Step 2]: add your Lambertian lighting parameters here
vec3 Id[5] = vec3[5] (vec3(0.8f, 0.8f, 1.f), vec3(0.6f, 0.5f, 1.f), vec3(0.2f, 0.5f, 0.5f), vec3(0.6f, 0.6f, 0.5f), vec3(0.5f, 0.3f, 0.3f));
vec3 Ia[5] = vec3[5] (vec3(0.6f, 0.6f, 0.6f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f));
vec3 Is[5] = Id; //light color

const float ka = 0.05f;
const float kd = 0.5f;
const float ks = 0.8f;

void main()							
{		
	vec3 amb_color = vec3(0.f, 0.f, 0.f);
	vec3 diff_color = vec3(0.f, 0.f, 0.f);
	vec3 spec_color = vec3(0.f, 0.f, 0.f);

	for (int i=0; i<lightPosArray.length(); i++){
		amb_color += ka*Ia[i];

		vec3 l = normalize((lightPosArray[i]-vtx_pos));
		
		float l_dot_n = dot(l, nml);
		float diff = max(0.f, l_dot_n);
		diff_color += kd*diff*Id[i]*(vtx_color.rgb);

		vec3 r = (-l) + 2*(dot(l,nml))*nml; //addition: following reflection calculate as in slides — -light + 2 * (light (dot) normal) * normal
		float spec = ks*pow(max(0.f, dot(normalize(position.xyz), normalize(r))), shiny); // following specularity calculations from slide — maximum of 0 and dot product of position and reflection, to the power of shiny-ness
		spec_color += spec*Is[i];
	}

	vec4 diff_final = vec4(diff_color, 1.f);

	vec4 spec_final = vec4(spec_color, 1.f);
	
	vec4 amb_final = vec4(amb_color, 1.f);

	frag_color = amb_final + diff_final + spec_final;
}	