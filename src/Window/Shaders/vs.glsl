#version 330 core

// Vertex attribute inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;

// Outputs from the vertex shader
out vec2 texturePos;

void main() {
  gl_Position = vec4(vertexPosition, 1.0);
  texturePos = vertexUV;
}
