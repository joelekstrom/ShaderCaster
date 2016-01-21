#import <stdio.h>
#import <GLFW/glfw3.h>
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
"    out_color = vec4(1.0, gl_FragCoord.x / buffer_size.x, mouse_position.y, 1.0);"
"}";

GLuint compile_shader(const char *src, GLenum shader_type);
GLuint create_program(GLuint vertex_shader, GLuint fragment_shader);

void print_GL_version(GLFWwindow *window);
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
	
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		printf("%.2f\n", ypos);
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

void print_GL_version(GLFWwindow *window)
{
	int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version: %d.%d.%d\n", major, minor, revision);
}

GLuint compile_shader(const char *src, GLenum shader_type)
{
	// Compile the shader
 	GLuint shader = glCreateShader(shader_type);
 	glShaderSource(shader, 1, &src, NULL);
 	glCompileShader(shader);
 	// Check the result of the compilation
 	GLint test;
 	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	
 	if(!test) {
		printf("Shader compilation failed with this message:\n");
		char compilation_log[512];
		glGetShaderInfoLog(shader, sizeof(compilation_log), NULL, (GLchar *)&compilation_log);
 		printf("%s", compilation_log);
 		glfwTerminate();
 		exit(-1);
 	}

 	return shader;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader)
{
    // Attach the above shader to a program
  	GLuint shaderProgram = glCreateProgram();
  	glAttachShader(shaderProgram, vertex_shader);
 	glAttachShader(shaderProgram, fragment_shader);

 	// Flag the shaders for deletion
 	glDeleteShader(vertex_shader);
 	glDeleteShader(fragment_shader);

 	// Link and use the program
 	glLinkProgram(shaderProgram);
 	glUseProgram(shaderProgram);
 
 	return shaderProgram;
}
