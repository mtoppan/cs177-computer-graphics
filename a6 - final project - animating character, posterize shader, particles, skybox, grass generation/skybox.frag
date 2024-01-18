#version 330 core

uniform samplerCube tex_spring;
uniform samplerCube tex_snow;
uniform samplerCube tex_sand;
uniform int weather_type;

in vec3 sampleCoord;

out vec4 frag_color;

vec4 posterize(vec4 color) 
{
    if (color.r > 0.8)
    {
        color.r = 1.0;
    }
    else if (color.r > 0.6)
    {
        color.r = 0.8;
    }
    else if (color.r > 0.4)
    {
        color.r = 0.6;
    }
    else if (color.r > 0.2)
    {
        color.r = 0.4;
    }
    else
    {
        color.r = 0.2;
    }
    if (color.g > 0.8)
    {
        color.g = 1.0;
    }
    else if (color.g > 0.6)
    {
        color.g = 0.8;
    }
    else if (color.g > 0.4)
    {
        color.g = 0.6;
    }
    else if (color.g > 0.2)
    {
        color.g = 0.4;
    }
    else
    {
        color.g = 0.2;
    }
    if (color.b > 0.8)
    {
        color.b = 1.0;
    }
    else if (color.b > 0.6)
    {
        color.b = 0.8;
    }
    else if (color.b > 0.4)
    {
        color.b = 0.6;
    }
    else if (color.b > 0.2)
    {
        color.b = 0.4;
    }
    else
    {
        color.b = 0.2;
    }
    return color;
}

void main() {
    // Use the appropriate texture for the given weather type
    switch (weather_type) {
        case 0:
            frag_color = texture(tex_spring, sampleCoord);
            break;
        case 1:
            frag_color = texture(tex_snow, sampleCoord);
            break;
        case 2:
            frag_color = texture(tex_sand, sampleCoord);
            break;
    }

    // Add in toon effect
    //frag_color = posterize(frag_color);

    // Make sure skybox is behind everything else
    // (Depth test must be set to GL_LEQUAL for this to work, which should be handled by skybox.h)
    gl_FragDepth = 1;
}
