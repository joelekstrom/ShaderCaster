#version 150
uniform vec2 mouse_position;
uniform ivec2 buffer_size;
out vec4 out_color;

void main() {
	// Get mouse position in screen space (Y-axis is bottm up)
    vec2 mouse_sp = vec2(mouse_position.x, 1.0 - mouse_position.y);
	vec2 rel_fragcoord = vec2(gl_FragCoord) / buffer_size;
    float mouse_distance = sqrt(pow(rel_fragcoord.y - mouse_sp.y, 2) + pow(mouse_sp.x - rel_fragcoord.x, 2));
    out_color = vec4(1.0, rel_fragcoord.x, 1.0 - mouse_distance * 4.0, 1.0);
}
