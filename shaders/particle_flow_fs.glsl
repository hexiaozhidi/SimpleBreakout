#version 330 core

#define NUM_TEXTURES 4

in vec2 TexCoord;
flat in int TextureIndex;
in vec4 TintColor;

out vec4 FragColor;

uniform sampler2D particleTexture[NUM_TEXTURES];

void main() {
    FragColor = texture(particleTexture[TextureIndex % NUM_TEXTURES], TexCoord) * TintColor;
}