#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stb_image.h>

static int WIDTH = 800;
static int HEIGHT = 600;

static void processInput(GLFWwindow *window);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static float radians(float degrees);

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

static int program = 0;
static int shader = 0;

static int success;
static char info[512];
static char *source = NULL;

static unsigned int vao;
static unsigned int vbo;

static unsigned int texture;

static vec3 position;

static float last_frame = 0.0f;
static float delta = 0.0f;

static mat4x4 model;
static mat4x4 view;
static mat4x4 projection;

static float angle = 0.0f;

static float speed = 3.0f;

static int width = 0;
static int height = 0;
static int nr_channels;

static unsigned char *data;

static vec3 camera_pos;
static vec3 camera_front;
static vec3 camera_up;

static int first_mouse = 1;

static float yaw = -90.0f;
static float pitch = -0.0f;
static float last_x = 800.0f / 2.0f;
static float last_y = 600.0f / 2.0f;
static float fov = 45.0f;

static char *loc_load_source(const char *filename);

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    +0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    +0.5f, +0.5f, -0.5f,  1.0f, 1.0f,
    +0.5f, +0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, +0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f, +0.5f,  0.0f, 0.0f,
    +0.5f, -0.5f, +0.5f,  1.0f, 0.0f,
    +0.5f, +0.5f, +0.5f,  1.0f, 1.0f,
    +0.5f, +0.5f, +0.5f,  1.0f, 1.0f,
    -0.5f, +0.5f, +0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, +0.5f,  0.0f, 0.0f,

    -0.5f, +0.5f, +0.5f,  1.0f, 0.0f,
    -0.5f, +0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, +0.5f,  0.0f, 0.0f,
    -0.5f, +0.5f, +0.5f,  1.0f, 0.0f,

    +0.5f, +0.5f, +0.5f,  1.0f, 0.0f,
    +0.5f, +0.5f, -0.5f,  1.0f, 1.0f,
    +0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    +0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    +0.5f, -0.5f, +0.5f,  0.0f, 0.0f,
    +0.5f, +0.5f, +0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    +0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    +0.5f, -0.5f, +0.5f,  1.0f, 0.0f,
    +0.5f, -0.5f, +0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, +0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, +0.5f, -0.5f,  0.0f, 1.0f,
    +0.5f, +0.5f, -0.5f,  1.0f, 1.0f,
    +0.5f, +0.5f, +0.5f,  1.0f, 0.0f,
    +0.5f, +0.5f, +0.5f,  1.0f, 0.0f,
    -0.5f, +0.5f, +0.5f,  0.0f, 0.0f,
    -0.5f, +0.5f, -0.5f,  0.0f, 1.0f
  };

  vec3 positions[10] = {
    0.0f, 0.0f, 0.0f,
    2.0f, 5.0f, -15.0f,
    -1.5f, -2.2f, -2.5f,
    -3.8f, -2.0f, -12.3f,
    2.4f, -0.4f, -3.5f,
    -1.7f,  3.0f, -7.5f,
    1.3f, -2.0f, -2.5f,
    1.5f,  2.0f, -2.5f,
    1.5f,  0.2f, -1.5f,
    -1.3f,  1.0f, -1.5f
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  stbi_set_flip_vertically_on_load(1);

  data = stbi_load("container.jpg", &width, &height, &nr_channels, 0);

  if (!data) {
    fprintf(stderr, "Was unable to load image.\n");
    return -1;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  mat4x4_identity(model);
  mat4x4_identity(view);
  mat4x4_identity(projection);

  camera_pos[0] = 0.0f;
  camera_pos[1] = 0.0f;
  camera_pos[2] = 3.0f;

  camera_front[0] = 0.0f;
  camera_front[1] = 0.0f;
  camera_front[2] = -1.0f;

  camera_up[0] = 0.0f;
  camera_up[1] = 1.0f;
  camera_up[2] = 0.0f;

  glEnable(GL_DEPTH_TEST);

  /* render loop */
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    float current_frame = glfwGetTime();
    delta = current_frame - last_frame;
    last_frame = current_frame;
    float time = glfwGetTime();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    mat4x4_identity(view);
    /*mat4x4_translate(view, 0.0f, 0.0f, -3.0f);*/
    vec3 tmp;
    vec3_add(tmp, camera_pos, camera_front);
    mat4x4_look_at(view, camera_pos, tmp, camera_up);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const float *)view);

    mat4x4_identity(projection);
    mat4x4_perspective(projection, 45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const float *)projection);

    glBindVertexArray(vao);

    angle += speed * delta;

    int i = 0;

    for (i=0; i<10; i++) {
      mat4x4_identity(model);
      mat4x4_translate(model, positions[i][0], positions[i][1], positions[i][2]);
      if (i == 0) {
        mat4x4_rotate_Y(model, model, angle);
        mat4x4_rotate_X(model, model, angle);
      }
      glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const float *)model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glUseProgram(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

static void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  float camera_speed = 2.5f * delta;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera_pos[0] += camera_speed * camera_front[0];
    camera_pos[1] += camera_speed * camera_front[1];
    camera_pos[2] += camera_speed * camera_front[2];
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera_pos[0] -= camera_speed * camera_front[0];
    camera_pos[1] -= camera_speed * camera_front[1];
    camera_pos[2] -= camera_speed * camera_front[2];
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

    vec3 tmp;

    vec3_mul_cross(tmp, camera_front, camera_up);
    vec3_norm(tmp, tmp);

    camera_pos[0] -= tmp[0] * camera_speed;
    camera_pos[1] -= tmp[1] * camera_speed;
    camera_pos[2] -= tmp[2] * camera_speed;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

    vec3 tmp;

    vec3_mul_cross(tmp, camera_front, camera_up);
    vec3_norm(tmp, tmp);

    camera_pos[0] += tmp[0] * camera_speed;
    camera_pos[1] += tmp[1] * camera_speed;
    camera_pos[2] += tmp[2] * camera_speed;
  }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

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

static float radians(float degrees) {
  return degrees * M_PI / 180.0f;
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

  if (first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = 0;
  }

  float offset_x = xpos - last_x;
  float offset_y = last_y - ypos;

  last_x = xpos;
  last_y = ypos;

  float sensitivity = 0.1f;

  offset_x *= sensitivity;
  offset_y *= sensitivity;

  yaw += offset_x;
  pitch += offset_y;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }

  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  vec3 front;

  front[0] = cosf(radians(yaw) * cos(radians(pitch)));
  front[1] = sinf(radians(pitch));
  front[2] = sinf(radians(yaw) * cos(radians(pitch)));

  vec3_norm(camera_front, front);

}
