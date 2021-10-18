#ifndef SHADERCODE_H
#define SHADERCODE_H

#include <string>

const std::string sprite_vShaderCode(R"EOF(
    #version 330 core

    layout (location = 0) in vec2 AttribPosition;
    layout (location = 1) in vec2 AttribTexCoord;

    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 projection;

    void main() {
        TexCoord = AttribTexCoord;

        gl_Position = projection * model * vec4(AttribPosition, 0.0, 1.0);
    }
    )EOF");

const std::string sprite_fShaderCode(R"EOF(
    #version 330 core

    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D spriteTexture;
    uniform vec4 tintColor;

    void main() {
        FragColor = texture(spriteTexture, TexCoord) * tintColor;
    }
    )EOF");

const std::string particle_flow_vShaderCode(R"EOF(
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
    )EOF");

    const std::string particle_flow_fShaderCode(R"EOF(
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
    )EOF");

const std::string post_processing_vShaderCode(R"EOF(
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
    )EOF");

const std::string post_processing_fShaderCode(R"EOF(
    #version 330 core

    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D postProcessingTexture;
    uniform float blurKernel[9];
    uniform float edgeKernel[9];
    uniform float time;
    uniform float grayscaleFactor;
    uniform bool shake;
    uniform bool blur;
    uniform bool chaos;
    uniform bool confusion;

    void main() {
        FragColor = vec4(vec3(0.0), 1.0);
        if (blur) {
		    int windowSize = 7;
		    float weight = 1.0 / (windowSize * windowSize);
		    vec2 texelSize = 1.0 / textureSize(postProcessingTexture, 0);
            for (int i = 0; i < 7; ++i)
	            for (int j = 0; j < 7; ++j) {
		            vec2 deltaTexCoord = vec2(i - 3, j - 3) * texelSize;
		            vec2 sampleTexCoord = TexCoord + deltaTexCoord;
		            vec3 sampleColor = texture(postProcessingTexture, sampleTexCoord).rgb;
		            FragColor.rgb += weight * sampleColor;
	            }
        } else if (chaos) {
            vec2 texelSize = 1.0 / textureSize(postProcessingTexture, 0);
            for (int i = 0; i < 3; ++i)
	            for (int j = 0; j < 3; ++j) {
		            vec2 deltaTexCoord = vec2(i - 1, j - 1) * texelSize;
		            vec2 sampleTexCoord = TexCoord + deltaTexCoord;
		            vec3 sampleColor = texture(postProcessingTexture, sampleTexCoord).rgb;
		            FragColor.rgb += sampleColor * edgeKernel[i * 3 + j];
	            }
		    float theta = 1.2 * time;
		    FragColor.rgb = (0.5 - FragColor.rgb) * (sin(theta) + sin(3.0 * theta) / 3.0 + sin(5.0 * theta) / 5.0 + sin(7.0 * theta) / 7.0) + 0.5;
        } else if (shake) {
            vec2 texelSize = 1.0 / textureSize(postProcessingTexture, 0);
            for (int i = 0; i < 3; ++i)
	            for (int j = 0; j < 3; ++j) {
		            vec2 deltaTexCoord = vec2(i - 1, j - 1) * texelSize;
		            vec2 sampleTexCoord = TexCoord + deltaTexCoord;
		            vec3 sampleColor = texture(postProcessingTexture, sampleTexCoord).rgb;
		            FragColor.rgb += sampleColor * blurKernel[i * 3 + j];
			    }
        } else {
            FragColor.rgb = texture(postProcessingTexture, TexCoord).rgb;
		    FragColor.rgb = mix(vec3(0.299 * FragColor.r + 0.587 * FragColor.g + 0.114 * FragColor.b), FragColor.rgb, grayscaleFactor);
	    }
    }
    )EOF");

const std::string text_vShaderCode(R"EOF(
    #version 330 core

    layout (location = 0) in vec2 AttribPosition;
    layout (location = 1) in vec2 AttribTexCoord;

    out vec2 TexCoord;

    uniform mat4 projection;

    void main() {
        TexCoord = AttribTexCoord;

        gl_Position = projection * vec4(AttribPosition, 0.0, 1.0);
    }
    )EOF");

const std::string text_fShaderCode(R"EOF(
    #version 330 core

    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D glyphTexture;
    uniform vec4 color;

    void main() {
        FragColor = vec4(color.rgb, texture(glyphTexture, TexCoord).r);
    }
    )EOF");

#endif