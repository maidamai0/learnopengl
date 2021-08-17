#version 400

// in vec4 Color;
in vec2 TexCoord;
// in vec4 Color;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture0;

void main() { FragColor = texture(texture0, TexCoord); }