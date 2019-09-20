#include <iostream>
#include <GL/glew.h>
#include <display.h>
#include <shader.h>
#include <mesh.h>

int main(int argc, char *argv[])
{

  Display display(800, 600, "Window!");

  Shader shader("data/default");

  Vertex vertices[] = {
    Vertex(glm::vec3(-0.5, -0.5, 0.0)),
    Vertex(glm::vec3(+0.5, -0.5, 0.0)),
    Vertex(glm::vec3(+0.0, +0.5, 0.0)),
  };

  Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]));

  std::cout << "Hi!" << std::endl;

  while (!display.IsClosed()) 
  {
    display.Clear(0.1f, 0.1f, 0.1f, 1.0f);
    shader.Bind();

    mesh.Draw();

    display.Update();
  }

  return 0;
}
