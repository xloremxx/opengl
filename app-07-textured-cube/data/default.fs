#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
  //FragColor = vec4(0.9f, 0.9f, 0.5f, 1.0f);
  FragColor = texture(ourTexture, TexCoord);
}
