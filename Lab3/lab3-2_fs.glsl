// Build on your code from Lab 1 here
#version 450

uniform sampler2D tex_sampler;
uniform samplerCube env_sampler;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

in vec3 f_position;
in vec3 f_normal;
in vec2 texture_coord;

out vec3 frag_colour;

vec3 reflectivve = reflect(f_position, normalize(f_normal));
vec4 text = texture(env_sampler, reflectivve);
vec3 sum = vec3(0.0f, 0.0f, 0.0f);

vec3 lambertian_brdf()
{
  vec3 p_albedo = vec3(1.0, 1.0, 0.0);
  float PI = 3.14;
  vec3 lambertian = vec3(p_albedo/PI);
  return lambertian;
}

void main () {
	//frag_colour = (normal + 1.0) * 0.5;
	for (int i = 0; i < light_count; ++i )
  {
    // Extracting the light direction and normalize it
    vec3 light_direction = normalize(light_position[i] - f_position);
    
    // Normalizing the light color
    vec3 normal_light_colour = vec3(normalize(light_colour[i]));

    sum = sum + vec3(   normal_light_colour *
                        lambertian_brdf() *
                        max(dot(light_direction, normalize(f_normal)), 0.0f) );
  }
	frag_colour = 0.8*text.rgb + sum*0.2;
}

// Note: Task 3.1 said to copy code from 2.1cpp but lightning is not
//		 implimented there. So i just set frag colour to text.rgb
//		 And brdf was not implimented untill task 2.2 also so i did not
//		 add that aswell. This will be added in task 3.2 instead