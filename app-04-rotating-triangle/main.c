#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int WIDTH = 800;
static int HEIGHT = 600;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static int program = 0;
static int shader = 0;

static int success;
static char info[512];
static char *source = NULL;

static unsigned int vao;
static unsigned int vbo;

static vec3 position;

static float last_frame = 0.0f;

static mat4x4 model;

static float angle = 0.0f;

static float speed = 3.0f;

static char *loc_load_source(const char *filename) {
  char *source = NULL;
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "Was unable to read '%s'.\n", filename);
    return NULL;
  }
  /* Go to the end of the file. */
  if (fseek(fp, 0L, SEEK_END) == 0) {
    /* Get the size of the file. */
    long bufsize = ftell(fp);
    if (bufsize == -1) { /* Error */ }

    /* Allocate our buffer to that size. */
    source = malloc(sizeof(char) * (bufsize + 1));

    /* Go back to the start of the file. */
    if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

    /* Read the entire file into memory. */
    size_t newLen = fread(source, sizeof(char), bufsize, fp);
    if ( ferror( fp ) != 0 ) {
      fputs("Error reading file", stderr);
    } else {
      source[newLen++] = '\0'; /* Just to be safe. */
    }
  }
  fclose(fp);

  return source;
}

int main(int argc, char *argv[])
{
  /* initialize */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

  /* create window */
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Was unable to create the window.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* load glad */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    fprintf(stderr, "Was unable to initialize GLAD.\n");
    return -1;
  }    

  /* create shader program */
  program = glCreateProgram();

  /* create vertex shader */
  shader = glCreateShader(GL_VERTEX_SHADER);
  source = loc_load_source("data/default.vs");
  glShaderSource(shader, 1, (const char **)&source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    fprintf(stderr, "Was unable to compile vertex shader: %s\n", info);
    return -1;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);

  /* create fragment shader */
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  source = loc_load_source("data/default.fs");
  glShaderSource(shader, 1, (const char **)&source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    fprintf(stderr, "Was unable to compile fragment shader: %s\n", info);
    return -1;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);

  /* link the program */
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info);
    fprintf(stderr, "Was unable to link program: %s\n", info);
    return -1;
  }

  free(source);

  /* set up vertex data */
  float vertices[] = {

    /* triangle */
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f,

  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  mat4x4_identity(model);

  /* render loop */
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    float current_frame = glfwGetTime();
    float delta = current_frame - last_frame;
    last_frame = current_frame;
    float time = glfwGetTime();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    mat4x4_identity(model);
    mat4x4_rotate_Y(model, model, angle);

    angle += speed * delta;

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const float *)model);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
