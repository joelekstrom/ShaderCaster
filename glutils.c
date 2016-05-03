#include "glutils.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(char *file_name)
{
	char *buffer = 0;
	long length;
	FILE *f = fopen(file_name, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length+1);
		if (buffer) {
			fread (buffer, 1, length, f);
			buffer[length] = '\0';
		}
		fclose (f);
	}
	
	if (buffer == 0) {
		printf("Error reading shader: %s", file_name);
		exit(1);
	}
	
	return buffer;
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
 		exit(1);
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
