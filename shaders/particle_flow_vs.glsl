#version 330 core

layout (location = 0) in vec2 AttribPosition;
layout (location = 1) in vec2 AttribTexCoord;
layout (location = 2) in int AttribInstancedTextureIndex;
layout (location = 3) in mat4 AttribInstancedModel;
layout (location = 7) in vec4 AttribInstancedTintColor;

out vec2 TexCoord;
flat out int TextureIndex;
out vec4 TintColor;

uniform mat4 projection;

void main() {
    TexCoord = AttribTexCoord;
    TextureIndex = AttribInstancedTextureIndex;
    TintColor = AttribInstancedTintColor;

    gl_Position = projection * AttribInstancedModel * vec4(AttribPosition, 0.0, 1.0);
}