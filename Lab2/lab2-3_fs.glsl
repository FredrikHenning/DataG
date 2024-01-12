#version 440

out vec3 frag_colour;
vec3 sum = vec3(0.0, 0.0, 0.0);

in vec3 f_normal;
in vec3 f_position;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

float PI = 3.14;

vec3 lambertian_brdf()
{
  vec3 p_albedo = vec3(1.0, 1.0, 0.0);
  vec3 lambertian = vec3(p_albedo/PI);

  return lambertian;
}

vec3 gloss( vec3 in_direction, vec3 out_direction, vec3 normal ){
    vec3 pg = vec3(1.0f, 1.0f, 0.0f);
    float s = 100.0f;

    vec3 halfp = normalize(in_direction + out_direction);  // Represent the halfway point of the view and light source

    vec3 glos = vec3 ( pg * ((8 + s)/(8*PI)) * pow(dot(normal, halfp), s) );
    return glos;
}
vec3 blinn_phong_brdf( vec3 in_direction, vec3 out_direction, vec3 normal)
{
    float kL = 0.5f;
    float kG = 0.5f;

    vec3 blin_phon = vec3( kL * lambertian_brdf() + kG * gloss(in_direction, out_direction, normal) );
    return blin_phon;
}

void main () {

  
  for (int i = 0; i < light_count; ++i )
  {
    // Extracting the light direction and normalize it
    vec3 light_direction = normalize(light_position[i] - f_position);
    
    // Normalizing the light color
    vec3 normal_light_colour = vec3(normalize(light_colour[i]));

    sum = sum + vec3(   normal_light_colour *
                        blinn_phong_brdf(light_direction, -f_position, normalize(f_normal)) *
                        max(dot(light_direction, normalize(f_normal)), 0.0f) );
  }

  frag_colour = sum * 5;
}

// (Old) Note to self: now not everything NEEDED to be normalized, be careful the next time
//               if more needs to be normalizes.
