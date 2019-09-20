#include <iostream>
#include <GL/glew.h>
#include <display.h>

int main(int argc, char *argv[])
{

  Display display(800, 600, "Window!");

  std::cout << "Hi!" << std::endl;

  while (!display.IsClosed()) 
  {
    display.Clear(0.1f, 0.1f, 0.1f, 1.0f);

    display.Update();
  }

  return 0;
}
