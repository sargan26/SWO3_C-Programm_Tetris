#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

_Noreturn
static
void glfw_error(void) {
	const char * msg = NULL;
	const int code = glfwGetError(&msg);
	assert(code); //otherwise, logic error
	if(msg) fprintf(stderr, "GLFW-ERROR: %s.\n", msg);
	abort();
}

static
const char * last_gl_error(void) {
	//error codes from: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
	switch(glGetError()) {
		case GL_NO_ERROR: return NULL;
		case GL_INVALID_ENUM: return "An unacceptable value is specified for an enumerated argument";
		case GL_INVALID_VALUE: return "A numeric argument is out of range";
		case GL_INVALID_OPERATION: return "The specified operation is not allowed in the current state";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "The framebuffer object is not complete";
		case GL_OUT_OF_MEMORY: return "There is not enough memory left to execute the command";
		case GL_STACK_UNDERFLOW: return "An attempt has been made to perform an operation that would cause an internal stack to underflow";
		case GL_STACK_OVERFLOW: return "An attempt has been made to perform an operation that would cause an internal stack to overflow";
		default: abort();
	}
}

_Noreturn
static
void gl_error(void) {
	const char * msg = last_gl_error();
	assert(msg);
	fprintf(stderr, "OGL-ERROR: %s.\n", msg);
	abort();
}

static
GLuint create_shader(GLenum type, const char * src) {
	GLuint shader = glCreateShader(type);
	if(!shader) gl_error();

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		char info[512];
		glGetShaderInfoLog(shader, sizeof(info), NULL, info);
		fprintf(stderr, "OGL-ERROR: Shader creation failed: %s.\n", info);
		abort();
	}

	return shader;
}


static
GLFWwindow * window;

static
key_callback callback;

static
GLuint program, vao;

static
GLint loc_position, loc_color;

static
const char * vertex_shader =
"#version 330 core\n"
"\n"
"uniform vec2 scaling;\n"
"uniform vec2 position;\n"
"\n"
"const vec2 offsets[] = vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));\n"
"\n"
"void main() {\n"
"\tgl_Position = vec4((position + offsets[gl_VertexID]) * scaling  - vec2(1, 1), 0, 1);\n"
"}\n"
;

static
const char * fragment_shader =
"#version 330 core\n"
"\n"
"uniform vec3 color;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"\tFragColor = vec4(color, 1);\n"
"}\n"
;

static
void on_key(GLFWwindow * win, int glfw_key, int scancode, int glfw_action, int modifiers) {
	(void)win;
	(void)scancode;
	(void)modifiers;

	key_t k;
	switch(glfw_key) {
		case GLFW_KEY_DOWN:  k = key_down;  break;
		case GLFW_KEY_LEFT:  k = key_left;  break;
		case GLFW_KEY_RIGHT: k = key_right; break;
		case GLFW_KEY_UP:    k = key_up;    break;
		case GLFW_KEY_Q:     k = key_q;     break;
		case GLFW_KEY_E:     k = key_e;     break;
		default: return;
	}

	action_t action;
	switch(glfw_action) {
		case GLFW_PRESS:   action = action_press;   break;
		case GLFW_REPEAT:  action = action_repeat;  break;
		case GLFW_RELEASE: action = action_release; break;
		default: return;
	}

	if(callback) callback(k, action);
}

extern
void renderer_init(int rows, int cols, key_callback cb) {
	assert(rows);
	assert(cols);
	callback = cb;

	if(!glfwInit()) glfw_error();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(400, 800, "Tetris", NULL, NULL);
	if(!window) glfw_error();

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetWindowAspectRatio(window, width, height); //enforce correct aspect ratio

	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "GLAD-ERROR: failed to initialize.\n");
		abort();
	}

	glfwSetKeyCallback(window, on_key);

	const GLuint vert = create_shader(GL_VERTEX_SHADER, vertex_shader);
	const GLuint frag = create_shader(GL_FRAGMENT_SHADER, fragment_shader);

	program = glCreateProgram();
	if(!program) gl_error();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glDeleteShader(vert);
	glDeleteShader(frag);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) gl_error();
	glUseProgram(program);

	loc_position = glGetUniformLocation(program, "position");
	assert(loc_position != -1);
	loc_color = glGetUniformLocation(program, "color");
	assert(loc_color != -1);
	const GLint loc_scaling = glGetUniformLocation(program, "scaling");
	assert(loc_scaling != -1);
	glUniform2f(loc_scaling, 2.f / cols, 2.f / rows);

	glGenVertexArrays(1, &vao);
	if(!vao) gl_error();
	glBindVertexArray(vao);
}

extern
void renderer_shutdown(void) {
	glUseProgram(0);
	glDeleteProgram(program);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glfwDestroyWindow(window);
	glfwTerminate();
}

extern
bool renderer_open(void) { return !glfwWindowShouldClose(window); }

extern
void renderer_begin_frame(void) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT); //clear frame buffer
}

extern
void renderer_end_frame(void) {
	const char * msg = last_gl_error();
	if(msg) fprintf(stderr, "OGL-ERROR: %s.\n", msg);

	glfwSwapBuffers(window); //push image to display
	glfwWaitEventsTimeout(0.25);
}

extern
void renderer_render(const position * pos, color col) {
	assert(pos);

	glUniform2f(loc_position, (float)pos->x, (float)pos->y);
	glUniform3f(loc_color, (col & color_red) ? 1.f : 0.f, (col & color_green) ? 1.f : 0.f, (col & color_blue) ? 1.f : 0.f);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
