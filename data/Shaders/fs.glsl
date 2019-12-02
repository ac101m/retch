#version 330 core

// Uniforms
uniform sampler2D texture0;

// Inputs
in vec2 texturePos;

// Outputs
out vec4 fragColor;

void main() {
  fragColor = texture(texture0, texturePos);
}
