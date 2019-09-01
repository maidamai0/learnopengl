#version 400

in vec4 Color;
in vec2 TexCoord;

out vec4 frag_colour;

uniform sampler2D outTexture;
uniform sampler2D outTexture1;

void main() {
    frag_colour = mix(texture(outTexture, TexCoord), texture(outTexture1, TexCoord), 0.2) * Color;
}
