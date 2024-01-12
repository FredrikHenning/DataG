#version 400
out vec4 o_fragment_color; // The final colour

vec3 sum = vec3(0.0);

// User input:
uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

// Camera parameters:
uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_SAMPLES 1

struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  float roughness;
  float reflection;
  float transmission;
  float ior;  
};

struct Intersection
{
	vec3 point;
	vec3 normal;
	Material material;
};

struct Sphere {
  float radius;
  vec3 center;
  Material material;
};

struct Plane {
  float offset;
  vec3 normal;
  Material material;  
};

struct Scene {
  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
	vec3 sun_position;
  float sun_brightness;
};




Scene scene;


void init( float sun_bright )
{
	// Hard-coded single point light source
	scene.sun_brightness = 1;
	scene.sun_position = vec3(6e3,  1e4, 1e4);
	
	// Initialise 5 spheres and a ground plane

	scene.spheres[0].center = vec3(0, 0.3, 0.5) ; 
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
	scene.spheres[0].material.color_glossy = vec3( 1 );
	scene.spheres[0].material.color_emission = vec3( 0 );
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 0.5; //reflective red ball
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;
  
	scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.5 * vec3( 0.0, 1.0, 0.0 );
	scene.spheres[1].material.color_glossy = vec3( 1 );
	scene.spheres[1].material.roughness = 10000;
	scene.spheres[1].material.color_emission = vec3( 0 );
	scene.spheres[1].material.reflection = 0.1;
	scene.spheres[1].material.transmission = 0.8; // glass green ball
	scene.spheres[1].material.ior = 1.4;

	scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3( 1.0, 0.0, 0.0 );
	scene.spheres[2].material.color_glossy = vec3( 1 );
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3( 1, 0, 0 ); // glowing red ball
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0;
	scene.spheres[2].material.ior = 1;

	scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.2, 0.2, 0.15 );
	scene.spheres[3].material.color_glossy = vec3( 1 );
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3( 0 );
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0;
	scene.spheres[3].material.ior = 1;

	scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
	scene.spheres[4].material.color_glossy = vec3( 1 );
	scene.spheres[4].material.roughness = 5000;
	scene.spheres[4].material.color_emission = vec3( 0 );
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;

	scene.ground_plane[0].normal = vec3(0,1,0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
	scene.ground_plane[0].material.color_glossy = vec3( 0 );
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3( 0 );
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;  
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
	float emission_sky = 1*scene.sun_brightness;
	float emission_sun = 10*scene.sun_brightness*scene.sun_brightness;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = vec3(1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(scene.sun_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


float intersect(Ray ray, Sphere s) 
{
  ///\todo YOUR CODE GOES HERE.
  // Return closest distance t for a ray/sphere intersection.
  vec3 v = vec3(ray.origin - s.center);
  float a = dot(ray.dir, ray.dir);
  float b = dot(2*ray.dir, v);
  float c = dot(v, v) - s.radius*s.radius;
  float h = b*b - 4*a*c;
  if(h<0)
  {
	return 0;
  }
  h = sqrt(h);
  float t = (-b - h)/(2*a);
  return max(t,0);

  /* vec3 d = vec3(ray.origin - s.center);
  float b = dot(d, ray.dir);
  float c = dot(d, d) - s.radius*s.radius;
  float h = b*b - c;
  if(h <= 0)
  {
	return 0;
  }
  h = sqrt(h);
  float t = -b-h;
  return max(t, 0); 
	
  return 0; */
}

float intersect(Ray ray, Plane p) 
{
  ///\todo YOUR CODE GOES HERE.
  // Return closest distance t for a ray/plane intersection.
  float d = p.offset;
  vec3 n = p.normal;
  vec3 p0 = ray.origin;
  vec3 w = ray.dir;
  float np0 = dot(n, p0);
  float nw = dot(n, w);
  float t = (d - np0)/nw;
  if(t <= 0)
  {
	return 1e32;
  }
  return t;
}


Intersection intersect( Ray ray)
{
  Intersection I;
  float t = 1e32; // closest hit so far along this ray
  int id = -1;
    
  //Check for intersection with spheres
  for (int i = 0; i < NUM_SPHERES; ++i) {
    float d = intersect(ray, scene.spheres[i]);
    if (d>0 && d<=t) // if sphere `i` is closer than `t`, update `t` and `id`
    {
      t = d; 
      id = i;

      ///\todo YOUR CODE GOES HERE
      // Populate I with all the relevant data.  `id` is the closest
      // sphere that was hit, and `t` is the distance to it.
	  I.point = vec3 (ray.origin + ray.dir*t);
	  /*I.normal = vec3((I.point*2)/(sqrt(I.point.x*I.point.x + I.point.y*I.point.y + I.point.z*I.point.z)));*/

	  I.normal = normalize(I.point - scene.spheres[i].center);

	  I.material.color_emission = scene.spheres[i].material.color_emission;
	  I.material.color_diffuse = scene.spheres[i].material.color_diffuse;
	  I.material.color_glossy = scene.spheres[i].material.color_glossy;
	  I.material.roughness = scene.spheres[i].material.roughness;
	  I.material.reflection = scene.spheres[i].material.reflection;
	  I.material.transmission = scene.spheres[i].material.transmission;
	  I.material.ior = scene.spheres[i].material.ior;

    }
  }

  //Check for intersection with planes
  {
    float d = intersect(ray,scene.ground_plane[0]);
    if (d>0 && d<=t) // if the plane is closer than `t`, update `t`
    {
      t=d;

      ///\todo YOUR CODE GOES HERE
      // Populate I with all the relevant data.

	  I.point = vec3 (ray.origin + ray.dir*t);
	  I.normal = scene.ground_plane[0].normal;
	  I.material.color_emission = scene.ground_plane[0].material.color_emission;
	  I.material.color_diffuse = scene.ground_plane[0].material.color_diffuse;
	  I.material.color_glossy = scene.ground_plane[0].material.color_glossy;
	  I.material.roughness = scene.ground_plane[0].material.roughness;
	  I.material.reflection = scene.ground_plane[0].material.reflection;
	  I.material.transmission = scene.ground_plane[0].material.transmission;
	  I.material.ior = scene.ground_plane[0].material.ior;
      
      // Adding a procedural checkerboard texture:
      I.material.color_diffuse = (mod(floor(I.point.x) + floor(I.point.z),2.0) == 0.0) ?
        scene.ground_plane[0].material.color_diffuse :
        vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
    }
  }

  //If no sphere or plane hit, we hit the sky instead
  if (t>1e20){
    I.point = ray.dir*t;
    I.normal = -ray.dir;
    vec3 sky = simple_sky(ray.dir); // pick color from sky function

    // Sky is all emission, no diffuse or glossy shading:
    I.material.color_diffuse = 0 * sky; 
    I.material.color_glossy = 0.0 * vec3( 1 );
    I.material.roughness = 1;
    I.material.color_emission = 0.3 * sky;
    I.material.reflection = 0.0;
    I.material.transmission = 0;
    I.material.ior = 1;

  }
  return I;
}

/*Implimenting brdf blin-phon - lambertian-----------------------------------------------------------------------*/
vec3 lambertian_brdf(Intersection I)
{
  vec3 pL = I.material.color_diffuse;
  vec3 lam = vec3(pL);
  
  return lam;
}

vec3 gloss( vec3 in_direction, vec3 out_direction, vec3 normal, Intersection I ){
   
    vec3 pG = vec3( I.material.color_glossy + I.material.color_diffuse ); 
    float f = I.material.roughness;

    vec3 h = normalize( in_direction + out_direction );
    float nDOTh = max( 0.0, dot(normal,h) );
    float expNH = pow( nDOTh, f );

    vec3 gl = vec3 ( pG * ((8 + f)/(8)) * expNH );

    return gl;
}

vec3 blinn_phong_brdf( vec3 in_direction, vec3 out_direction, vec3 normal, Intersection I)
{
    float kL = 0.5f;
    float kG = 0.5f;

    vec3 bp = vec3( kL * lambertian_brdf(I) + kG * gloss(in_direction, out_direction, normal, I) );

    return bp;
}
/*-------------------------------------------------------------------------------------------------*/

vec3 raycast(Ray ray) 
{
  vec3 color = vec3(0);

  Intersection isec = intersect(ray);

  ///\todo YOUR CODE GOES HERE: Compute a more interesting colour instead of
  // the one below!  Compute the shading, using the
  // position/normal/material data stored in isec; and
  // i_light_position, which is a point light source at the sun.

    color = isec.material.color_diffuse + isec.material.color_emission;

	vec3 blinn_phong_color = vec3(0);
	vec3 light_direction = vec3(normalize(scene.sun_position - isec.point));
	vec3 camera_direction = vec3(normalize(i_position - isec.point));
	if (isec.material.color_emission ==  vec3(0))
	{
		blinn_phong_color = isec.material.color_emission + 
							blinn_phong_color + 
							vec3( 
									blinn_phong_brdf( light_direction, camera_direction, normalize(isec.normal), isec ) * 
									max( 0.0f, dot(light_direction, normalize(isec.normal)) ) 
								);
	}
	else
	{
		return color;
	} 
	
  return blinn_phong_color;
}
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123 );
}

void main() {
	vec2 uv =  gl_FragCoord.xy - 0.5*i_window_size.xy;
	
	//init(i_light_position, 10.0 ); //i_mouse_state.y); 
	init( 10.0 );
	
	int num_samples=1;

		
	//crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist*i_mouse_state.w; 
	
	//basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;   
	vec3 cz = i_dir;  

	vec2 ipos = floor(uv * 10);
	vec3 random_nr = vec3(random(ipos));

	Ray ray;
	
	///\todo YOUR CODE GOES HERE: Compute the correct ray direction using gl_fragCoord and the camera vectors above.
	ray.origin = i_position;
	ray.dir = normalize((cx * uv.x  + cy * uv.y  + cz * i_focal_dist ) + random_nr) ;
	

	vec3 color = raycast(ray);
	
	//linear blend
	// o_fragment_color =  vec4((color),1);

	//gamma correction
	o_fragment_color = vec4( pow ( clamp(color.xyz/num_samples, 0., 1.), vec3(1./2.2)), 1.); 

	///\todo REMOVE THIS LINE after you have the triangles set up
	//o_fragment_color = vec4(1,0.9,0.8,1); 
}


