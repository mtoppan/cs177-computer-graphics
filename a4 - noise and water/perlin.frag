#version 330 core

// Parts 1a, 1b, 1c

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

in vec3 vtx_normal;
in vec3 vtx_pos;

out vec4 frag_color;

///////////// Part 1a /////////////////////
/* Create a function that takes in an xy coordinate and returns a 'random' 2d vector. (There is no right answer)
   Feel free to find a hash function online. Use the commented function to check your result */
vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	// Your implementation starts here
	// rand  = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    // rand = -1.0 + 3.5 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);

	//Based on the Book of Shaders random hash function
	
    vec2 rand_temp = vec2( dot(v,vec2(127.1,311.7)),
              dot(v,vec2(269.5,183.3)) );
    rand = -1.0 + 2.0*fract(sin(rand_temp)*43758.5453123);

	// Your implementation ends here

	return rand;
}

///////////// Part 1b /////////////////////
/*  Using i, f, and m, compute the perlin noise at point v */
float perlin_noise(vec2 v) 
{
	float noise = 0;

	// Your implementation starts here
	vec2 i = floor(v);
    vec2 f = fract(v);
	vec2 s = 3.0*(f*f) - 2.0*(f*f*f);

    noise = mix( mix( dot( hash2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
						dot( hash2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), 
						s.x), 
			mix( dot( hash2(i + vec2(0.0,1.0)), f - vec2(0.0,1.0) ), 
						dot( hash2(i + vec2(1.0,1.0)), f - vec2(1.0,1.0) ), 
						s.x ), 
				s.y );
	// Your implementation ends here

	return noise;
}

///////////// Part 1c /////////////////////
/*  Given a point v and an int num, compute the perlin noise octave for point v with octave num
	num will be greater than 0 */
float noiseOctave(vec2 v, int num)
{
	float sum = 0;
	// Your implementation starts here
	for (int i = 1; i<=num; i++){
		sum += pow(2, -i)*(perlin_noise(pow(2, i) * v));
	}
	// Your implementation ends here

	return sum;
}

void main()
{
	vec3 color = 0.5 + 0.5 * (noiseOctave(vtx_pos.xy, 6))  * vec3(1,1,1); // visualize perlin noise
	frag_color=vec4(color,1.f);
}