#version 330 core

layout (location = 0) in vec2 AttribPosition;
layout (location = 1) in vec2 AttribTexCoord;

out vec2 TexCoord;

uniform float time;
uniform bool shake;
uniform bool swing;
uniform bool chaos;
uniform bool confusion;

void main() {
    TexCoord = AttribTexCoord;
    if (swing) {
        float n = 2.0, theta = 0.05 * time;
        TexCoord.x += cos(theta) + cos(n * theta) / n;
        TexCoord.y += sin(theta) - sin(n * theta) / n;
    }
    else if (chaos) {
        float n = 47.0, theta = 0.15 * time;
        TexCoord.x += cos(theta) + cos(n * theta) / n;
        TexCoord.y += sin(theta) - sin(n * theta) / n;
    }
    else if (confusion)
        TexCoord = 1.0 - TexCoord;

    gl_Position = vec4(AttribPosition, 0.0, 1.0);
    if (shake)
        gl_Position.xy += 0.01 * vec2(cos(10.0 * time), sin(10.0 * time));
}