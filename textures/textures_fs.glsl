#version 400

in vec4 Color;
in vec2 TexCoord;

out vec4 frag_colour;

uniform sampler2D outTexture;

void main() {
    frag_colour = texture(outTexture, TexCoord) * Color;
}
