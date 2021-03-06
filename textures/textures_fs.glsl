#version 400

in vec4 Color;
in vec2 TexCoord;

out vec4 frag_colour;

uniform sampler2D outTexture;
uniform sampler2D outTexture1;

uniform float ratio;

void main() {
    frag_colour = mix(texture(outTexture, TexCoord),
                      texture(outTexture1, vec2(1.0 - TexCoord.x, TexCoord.y)),
                      ratio) *
                  Color;
}
