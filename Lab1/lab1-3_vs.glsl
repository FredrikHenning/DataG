#version 450
in vec3 vp;
out vec3 position;
uniform vec2 position_offset;

void main () {
  
	// Change this so that you don't just set `gl_Position` to `vp`, but
	// also add an offset from a `uniform` variable:
	gl_Position = vec4(vp.x + position_offset.x, vp.y + position_offset.y, vp.z, 1.0);

	// The color is fixed to the triangle
	position = vp;
	
	// The colour is fixed to the background/window
	//position.x = gl_Position.x;
	//position.y = gl_Position.y;

};
  
