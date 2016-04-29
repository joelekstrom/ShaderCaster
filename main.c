#include <GL/gl3w.h>
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

GLFWwindow *setup_window(int width, int height, const char *title);

int main()
{
    if (!glfwInit()) {
        return -1;
	}

	GLFWwindow *window = setup_window(640, 480, "Hellurr!!");

	// Create Vertex buffer object, containing our 4 vertices.
	// This allows us to draw a full screen quad, so we can
	// later on do raycasting in the fragment shader
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Compile shaders. The vertex shader is just pass-through
	char *f_shader_source = read_file("default.frag");
	GLuint v_shader = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	GLuint f_shader = compile_shader((const char *)f_shader_source, GL_FRAGMENT_SHADER);
	GLuint program = create_program(v_shader, f_shader);
	free((void *)f_shader_source);

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

    glfwMakeContextCurrent(window);

    if (gl3wInit()) {
        printf("failed to initialize OpenGL\n");
        return -1;
    }

    if (!gl3wIsSupported(3, 2)) {
        printf("OpenGL 3.2 not supported\n");
        return -1;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	return window;
}
