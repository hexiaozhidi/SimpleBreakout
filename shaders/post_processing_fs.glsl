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