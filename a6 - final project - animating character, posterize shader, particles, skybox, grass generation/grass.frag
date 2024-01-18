#version 330 core

/*default camera matrices. do not modify unless you know what you are doing.*/
uniform camera{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};
/*light related variables. do not modify unless you know what you are doing.*/
struct light
{
	ivec4 att;////0-type, 1-has_shadow, type: 0-directional, 1-point, 2-spot
	vec4 pos;
	vec4 dir;
	vec4 amb;
	vec4 dif;
	vec4 spec;
	vec4 atten;////0-const, 1-linear, 2-quad
	vec4 r;
};
uniform lights
{
	vec4 amb;
	ivec4 lt_att;	////lt_att[0]: lt num
	light lt[4];
};

const vec3 LightPosition = vec3(9,0, 10);

vec4 pColor;

/*shadow related texture and function, don't modify unless you know what you are doing*/
uniform sampler2D shadow_map;
float shadow(vec4 shadow_pos,vec3 normal,vec3 light_dir)
{
	vec3 proj_coord=shadow_pos.xyz/shadow_pos.w;
	proj_coord=proj_coord*.5f+.5f;
	
	float shadow=0.f;float dp=proj_coord.z;float step=1.f/512.f;
	float bias=max(.05f*(1.f-dot(normal,light_dir)),.005f);
	for(int i=-1;i<=1;i++)for(int j=-1;j<=1;j++){
		vec2 coord=proj_coord.xy+vec2(i,j)*step;
		float dp0=texture(shadow_map,coord).r;
		shadow+=dp>dp0+bias?0.2f:1.f;}shadow/=9.f;
	return shadow;
}

/*uniform variables*/
uniform float iTime;					////time
uniform sampler2D tex_spring;			////texture color
uniform sampler2D tex_snow;			////texture color
uniform sampler2D tex_sand;			////texture color
uniform sampler2D tex_alpha;
uniform int weather_type;

/*input variables*/
in vec3 vtx_normal;
in vec3 vtx_frg_pos;
in vec4 vtx_shadow_pos;
in vec4 vtx_uv;
in float totalSeconds;

in vec3 vertexPosition;
in vec3 tgent;

out vec4 frag_color;

void main()
{
	float alpha = texture(tex_alpha, vtx_uv.xy).x;
	if (alpha < 0.1) discard;

	// Use the appropriate texture for the given weather type
	vec4 vtx_color;
    switch (weather_type) {
        case 0:
            vtx_color = texture(tex_spring, vtx_uv.xy);
            break;
        case 1:
            vtx_color = texture(tex_snow, vtx_uv.xy);
            break;
        case 2:
            vtx_color = texture(tex_sand, vtx_uv.xy);
            break;
    }

	frag_color = vec4(vtx_color.xyz, alpha);
}