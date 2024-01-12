#version 450
in vec3 vp;
out vec3 position;

void main () {
  gl_Position = vec4 (vec3(vp), 1.0);
  position.z = gl_Position.z;
};
  
