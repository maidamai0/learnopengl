#version 400

// in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float ratio;

void main() {
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(
        texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), ratio);
}