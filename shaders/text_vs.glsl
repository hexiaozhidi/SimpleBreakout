#version 330 core

layout (location = 0) in vec2 AttribPosition;
layout (location = 1) in vec2 AttribTexCoord;

out vec2 TexCoord;

uniform mat4 projection;

void main() {
    TexCoord = AttribTexCoord;

    gl_Position = projection * vec4(AttribPosition, 0.0, 1.0);
}