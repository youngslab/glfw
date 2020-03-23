//#include <EGL/egl.h>
//#include <GLES3/gl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static const char *vertex_shader_text =
    " #version 300 es\n"
    " layout(location = 0) in vec4 vPosition;\n"
    " void main()\n"
    " {\n"
    "    gl_Position = vPosition;\n"
    " }\n";

static const char *fragment_shader_text =
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
    "}\n";

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
			 int mods) {
  fprintf(stdout, "key pressed: %d\n", key);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void _post_call_callback_default(const char *name, void *funcptr, int len_args,
				 ...) {
  GLenum error_code;
  error_code = glad_glGetError();

  if (error_code != GL_NO_ERROR) {
    fprintf(stderr, "ERROR %d in %s\n", error_code, name);
  }
}

void checkGLError() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cout << "error: " << err;
  }
}

static void cursor_position_callback(GLFWwindow *window, double xpos,
				     double ypos) {
  fprintf(stdout, "cursor pos: %d, %d\n", xpos, ypos);
}

int main() {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  // glfwSwapInterval(1);

  // NOTE: OpenGL error checks have been omitted for brevity
  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  };

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  checkGLError();
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
	       g_vertex_buffer_data, GL_STATIC_DRAW);
  checkGLError();

  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);
  checkGLError();

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  checkGLError();
  glCompileShader(fragment_shader);
  checkGLError();

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  checkGLError();
  glAttachShader(program, fragment_shader);
  checkGLError();
  glLinkProgram(program);
  checkGLError();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    checkGLError();
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    checkGLError();
    glVertexAttribPointer(0, 3,
			  GL_FLOAT,  // 타입(type)
			  GL_FALSE,  // 정규화(normalized)?
			  0,	 // 다음 요소 까지 간격(stride)
			  (void *)0  // 배열 버퍼의 오프셋(offset; 옮기는 값)
			  );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGLError();
    glDisableVertexAttribArray(0);
    checkGLError();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

