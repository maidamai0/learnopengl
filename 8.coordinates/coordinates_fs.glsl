#version 400

// in vec4 Color;
in vec2 TexCoord;
// in vec4 Color;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float ratio;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), ratio);
}