#version 330 core

// Uniforms
uniform sampler2D texture0;

// Inputs
in vec2 texturePos;

// Outputs
out vec4 gl_FragColor;

void main() {
  gl_FragColor = texture(texture0, texturePos);
}
