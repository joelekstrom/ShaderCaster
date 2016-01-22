#import "glutils.h"
#import <stdlib.h>

GLfloat vertices[] = {
	-1.0, -1.0,
	1.0, -1.0,
	-1.0, 1.0,
	1.0, 1.0
};

const char *vertex_shader =
"#version 150\n"
"in vec4 in_vertex;"

"void main() {"
"    gl_Position = in_vertex;"
"}";


const char *fragment_shader =
"#version 150\n"
"uniform vec2 mouse_position;"
"uniform ivec2 buffer_size;"
"out vec4 out_color;"

"void main() {"
	// Get mouse position in screen space (Y-axis is bottm up)
"    vec2 mouse_sp = vec2(mouse_position.x, 1.0 - mouse_position.y);"
"	 vec2 rel_fragcoord = vec2(gl_FragCoord) / buffer_size;"
"    float mouse_distance = sqrt(pow(rel_fragcoord.y - mouse_sp.y, 2) + pow(mouse_sp.x - rel_fragcoord.x, 2));"
"    out_color = vec4(1.0, rel_fragcoord.x, 1.0 - mouse_distance * 4.0, 1.0);"
"}";

GLFWwindow *setup_window(int width, int height, const char *title);

int main()
{
    if (!glfwInit()) {
        return -1;
	}

	GLFWwindow *window = setup_window(640, 480, "Hellurr!!");
	print_GL_version(window);
    glfwMakeContextCurrent(window);

	// Create Vertex buffer object, containing our 4 vertices.
	// This allows us to draw a full screen quad, so we can
	// later on do raycasting in the fragment shader
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Compile shaders. The vertex shader is just pass-through
	GLuint v_shader = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	GLuint f_shader = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
	GLuint program = create_program(v_shader, f_shader);

	// Bind shader attributes with a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
  	glBindVertexArray(vao);
	
	GLint vertex_handle = glGetAttribLocation(program, "in_vertex");
	glVertexAttribPointer(vertex_handle, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertex_handle);

   	// Pass the frame buffer size to the fragment shader
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	GLint size_handle = glGetUniformLocation(program, "buffer_size");
   	glUniform2i(size_handle, width, height);

	GLint cursor_handle = glGetUniformLocation(program, "mouse_position");

	// Enable vsync
	glfwSwapInterval(1);
	
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glUniform2f(cursor_handle, xpos / 640, ypos / 480);
		
		// Draw our full screen quad
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Swap front and back buffers
        glfwSwapBuffers(window);
		glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}

GLFWwindow *setup_window(int width, int height, const char *title)
{
	// Make sure we run at least OpenGL 3.2
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(1);
    }
	
	return window;
}
