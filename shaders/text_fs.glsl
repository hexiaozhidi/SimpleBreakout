#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D glyphTexture;
uniform vec4 color;

void main() {
    FragColor = vec4(color.rgb, texture(glyphTexture, TexCoord).r);
}