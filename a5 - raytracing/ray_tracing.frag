#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
in vec2 fragCoord; 
out vec4 fragColor;

uniform sampler2D bufferTexture;

#define M_PI 3.1415925585

////data structures for ray tracing
struct camera{
    vec3 origin;
    vec3 horizontal;
    vec3 vertical;
    vec3 LowerLeftCorner;
};

struct ray{
    vec3 ori;
    vec3 dir;
};

struct sphere{
    vec3 ori;			////sphere center
    float r;			////sphere radius
    vec3 color;			////sphere color
};

struct light {
   vec3 position;		////point light position
   vec3 color;			////point light color
};
    
struct hit{
    float t;			////parameter in the ray function
    vec3 p;				////intersection point
    vec3 normal;		////normal on the intersection point
    vec3 color;			////color of the intersecting object
};

//////////// Random functions ///////////
float g_seed = 0.;

uint base_hash(uvec2 p) {
    p = 1103515245U*((p >> 1U)^(p.yx));
    uint h32 = 1103515245U*((p.x)^(p.y>>3U));
    return h32^(h32 >> 16);
}

void init_rand(in vec2 frag_coord, in float time) {
    g_seed = float(base_hash(floatBitsToUint(frag_coord)))/float(0xffffffffU)+time;
}

vec2 rand2(inout float seed) {
    uint n = base_hash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    uvec2 rz = uvec2(n, n*48271U);
    return vec2(rz.xy & uvec2(0x7fffffffU))/float(0x7fffffff);
}
/////////////////////////////////////////

const float minT = 0.001;
const float maxT = 1e8;
const int numberOfSampling = 50;

////if no hit is detected, return dummyHit
const hit dummyHit = hit(-1.0, vec3(0), vec3(0), vec3(0));

////calculate the ray for a given uv coordinate
ray getRay(camera c, vec2 uv)
{
    return ray(c.origin, c.LowerLeftCorner + uv.x * c.horizontal + uv.y * c.vertical - c.origin);
}

////TODO: implement ray-sphere intersection
hit hitSphere(const ray r, const sphere s)
{
    float delta = 0.f;
	////TODO: check whether r is intersecting with s by updating delta
	/*Your implementation*/
    vec3 r_origin = r.ori;
    vec3 r_dir = r.dir;

    vec3 s_center = s.ori;
    float s_radius = s.r;
    vec3 s_color = s.color;

    float A = dot(r_dir, r_dir);
    float B = 2*dot((r_origin - s_center), r_dir);
    float C = dot((r_origin - s_center), (r_origin - s_center)) - s_radius*s_radius;
    delta = B*B - 4*A*C;

    if(delta<0.0){
        // no solution, return dummy
        return  dummyHit;
    }
    else {
		hit h;
		h.color=s.color;

		////TODO: update other attributes of hit when an intersection is detected
		/*Your implementation*/

        float t_param = (-B - sqrt(delta)) / (2*A);
        vec3 intersect = r_origin + t_param*r_dir;
        vec3 norm = vec3((intersect.x - s_center.x)/s_radius, (intersect.y - s_center.y)/s_radius, (intersect.z - s_center.z)/s_radius);
        
        h.t = t_param;
        h.p = intersect;
        h.normal = norm;
        h.color = s_color;
		
        return h;
    }
}

////TODO: return the hit sphere with the smallest t
hit findHit(ray r, sphere[4] s) 
{
	hit h = dummyHit;
    ////TODO: traverse all the spheres and find the intersecting one with the smallest t value
	/*Your implementation*/
    float t_check = 10000;
    for (int i = 0; i < 4; i++){
        hit h_check = hitSphere(r, s[i]);
        if ((h_check.t < t_check) && (h_check.t > 0)){
            t_check = h_check.t;
            h = h_check;
        }
    }

	return h;
}

////TODO: calculate the pixel color for each ray
vec3 color(ray r, sphere[4] s, light[2] l)
{
    vec3 col = vec3(0);
    hit h = findHit(r, s);
    float ka = 0.05;
    float kd = 1;
    if(h.t > 0.){
		////TODO: traverse all the lights and calculate the color contribution from each of them
		////TODO: send an additional shadow ray for each light to implement the shadow effect
		/*Your implementation*/
        vec3 amb_color = vec3(0.f, 0.f, 0.f);
	    vec3 diff_color = vec3(0.f, 0.f, 0.f);

        amb_color += ka*h.color;

	    for (int i=0; i<2; i++){ //loop through all lights in scene
        //make a new ray from the point towards the light
            ray r_shadow = ray(l[i].position, normalize(h.p - l[i].position));
            hit h_shadow = findHit(r_shadow, s);
        
            if (length(h_shadow.p - h.p) <= 0.01){
                vec3 l_pos = l[i].position;
                vec3 l_col = l[i].color;

                vec3 v_pos = h.p;
                vec3 v_norm = h.normal;
                vec3 v_col = h.color;

		        vec3 l = normalize((l_pos-v_pos));
		
		        float l_dot_n = dot(l, v_norm);
		        float diff = max(0.f, l_dot_n);
		        diff_color += kd*diff*(l_col.rgb)*(v_col.rgb);
            }
	    }

	    col = amb_color + diff_color; //sum them all
    }

    
    return col;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    camera c = camera(vec3(0,15,50), vec3(5, 0, 0), vec3(0, 3, -3), vec3(-2.5, -1.5, -1));
    sphere s[4];
    s[0] = sphere(vec3(0, 0.6, -1), 0.6, vec3(0.8,0.2,0.2));
	s[1] = sphere(vec3(1.2, 0.4, -1), 0.4, vec3(0.2,0.9,0.2));
	s[2] = sphere(vec3(-1.2, 0.5, -1), 0.5, vec3(0.2,0.2,0.9));
    s[3] = sphere(vec3(0, -200, -1),200.0, vec3(0.5,0.5,0.5));

	light l[2];
	l[0] = light(vec3(-1, 3, 0.5), vec3(1));
	l[1] = light(vec3(0.5, 2, 1), vec3(1));
    vec3 resultCol = vec3(0);

    // Here I use i to get different seeds for each run
    init_rand(fragCoord, iTime);
    vec2 random = rand2(g_seed);
    ray r = getRay(c, uv + random/iResolution.xy);
    resultCol += color(r, s, l);
    
	// Output to screen
    fragColor = vec4((resultCol + float(iFrame-1) * texture(bufferTexture, uv).xyz)/float(iFrame), 1.);
}

void main() {
	mainImage(fragColor, fragCoord);
}
