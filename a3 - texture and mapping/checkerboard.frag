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

/*input variables*/
//// TODO: declare the input fragment attributes here
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;

void main()							
{						
	vec3 col = vec3(1.0);

	//// TODO: produce a checkerboard texture on the sphere with the input vertex uv
	int size = 10;
	float PI = 3.1415926538;
	vec3 n_pos = vtx_pos.xyz;
	normalize(n_pos);

	float u = (atan(n_pos[2], n_pos[0]) / (2*PI));
	float v = (atan(sqrt(vtx_pos[0]*vtx_pos[0] + vtx_pos[2]*vtx_pos[2]), vtx_pos[1]) / PI);
	
	int f_u = int(floor(u*size));
	int f_v = int(floor(v*size));
	int sum = f_u + f_v;

	int m = int(mod(sum, 2));

	
	if (m != 0){
		col = vec3(0.0);
	}

	frag_color = vec4(col, 1.0);
}	