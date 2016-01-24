#version 150
uniform vec2 mouse_position;
uniform ivec2 buffer_size;
out vec4 out_color;

float mouse_distance(vec2 relative_frag_coord) {
	// Get mouse position in screen space (Y-axis is bottom up, so flip Y)
    vec2 mouse_screen_space = vec2(mouse_position.x, 1.0 - mouse_position.y);
	return sqrt(pow(relative_frag_coord.y - mouse_screen_space.y, 2) + pow(relative_frag_coord.x - mouse_screen_space.x, 2));
}

void main() {
	// Get the relative fragment position (values between 0.0-1.0)
	vec2 r = vec2(gl_FragCoord) / buffer_size;
	
	// We want the colors to change within 0.2 distance from the mouse
	float max_distance = 0.2;
	float multiplier = 1.0 / max_distance;
	float relative_mouse_distance = 1.0 - mouse_distance(r) * multiplier;

	vec4 base_color = vec4(1.0, r.x, 0.0, 1.0);
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    out_color = mix(base_color, white, relative_mouse_distance);
}
