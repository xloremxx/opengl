#ifndef MY_SHADER_H
#define MY_SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{

  public:

    Shader(const std::string& fileName);

    void Bind();

    virtual ~Shader();

  protected:

  private:

    static const unsigned int NUM_SHADERS = 2;

    GLuint m_program;

    GLuint m_shaders[NUM_SHADERS];

};

#endif
