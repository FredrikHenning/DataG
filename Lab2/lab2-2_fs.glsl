#version 440

out vec3 frag_colour;
vec3 sum = vec3(0.0, 0.0, 0.0);

in vec3 f_normal;
in vec3 f_position;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];


vec3 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  vec3 p_albedo = vec3(1.0, 1.0, 0.0);
  float PI = 3.14;
  vec3 lambertian = vec3(p_albedo/PI);
  return lambertian;
}

void main () {

  
  for (int i = 0; i < light_count; ++i )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)

    // Extracting the light direction and normalize it
    vec3 light_direction = normalize(light_position[i] - f_position);
    
    // Normalizing the light color
    vec3 normal_light_colour = vec3(normalize(light_colour[i]));

    sum = sum + vec3(   normal_light_colour *
                        lambertian_brdf(light_direction, f_position, f_normal) *
                        max(dot(light_direction, f_normal), 0.0f) );
  }
  frag_colour = sum * 5;
}

// Note to self: now not everything NEEDED to be normalized, be careful the next time
//               if more needs to be normalizes.
