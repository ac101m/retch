#include "Window/Window.hpp"
using namespace retch;

// standard
#include <vector>


Window::Window(int const width, int const height, std::string const name) {

  // Create window
  this->gltWindow = std::shared_ptr<GLT::Window>(
    new GLT::Window(width, height, name));

  // Create shader program
  this->shader = std::shared_ptr<GLT::ShaderProgram>(
    new GLT::ShaderProgram({
      GLT::Shader(GL_VERTEX_SHADER, "data/Shaders/vs.glsl"),
      GLT::Shader(GL_FRAGMENT_SHADER, "data/Shaders/fs.glsl")}));

  // Create the frame buffer texture
  std::vector<unsigned char> data(width * height * 3);
  this->fbTexture = std::shared_ptr<GLT::Texture>(
    new GLT::Texture(width, height, data));

  // Set up texture filtering
  this->fbTexture->Parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  this->fbTexture->Parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Vertex positions, UVs and indices
  std::vector<GLT::vertex_t> vertices = {
    {glm::fvec3(-1,-1, 0), glm::fvec2(0.0001, 0.0001)},
    {glm::fvec3( 1,-1, 0), glm::fvec2(1.0001, 0.0001)},
    {glm::fvec3(-1, 1, 0), glm::fvec2(0.0001, 1.0001)},
    {glm::fvec3( 1, 1, 0), glm::fvec2(1.0001, 1.0001)}};
  std::vector<unsigned> const indices = {
    0, 1, 2, 3, 2, 1};

  // Create mesh to map the frame buffer texture to
  this->mesh = std::shared_ptr<GLT::Mesh>(
    new GLT::Mesh(vertices, indices, {*this->fbTexture}));

  this->gltWindow->EnableFpsCounter();
}


// Takes a 2D RGBA float buffer and displays it on the screen
void Window::Output(Buffer2D<glm::vec4>& frameBuffer) {

  // Convert incoming frame buffer from float RGBA to 8-bit RGB
  std::vector<unsigned char> rawData(
    frameBuffer.Width() * frameBuffer.Height() * 3);

  for(unsigned i = 0; i < frameBuffer.Width() * frameBuffer.Height(); i++) {
    rawData[(i * 3) + 0] = frameBuffer.data()[i].x * 255;
    rawData[(i * 3) + 1] = frameBuffer.data()[i].y * 255;
    rawData[(i * 3) + 2] = frameBuffer.data()[i].z * 255;
  }

  // Load the data onto the GPU via a texture
  this->fbTexture->SetData(
    frameBuffer.Width(), frameBuffer.Height(), rawData);

  // Make the draw call
  glm::mat4 m(1.0f);
  this->gltWindow->Draw(*this->mesh, *this->shader, m);
}


void Window::Refresh(void) {
  this->gltWindow->Refresh();
}


// Override mesh draw routine
void GLT::Mesh::Draw(
  GLT::Camera& camera,
  GLT::ShaderProgram& shader,
  glm::mat4& m) {

  // Bind textures to texture units
  std::string name = "texture0";
  for(unsigned i = 0; i < this->textures.size(); i++) {
    name[7] = 48 + i;
    shader.SetTexture(i, name, this->textures[i]);
  }

  // Draw the things
  this->vertexBuffer.Bind();
  glDrawElements(
    GL_TRIANGLES,
    this->vertexBuffer.GetIndexBufferLength(),
    GL_UNSIGNED_INT, 0);
  this->vertexBuffer.Unbind();

  // Set texture back to texture 0
  glActiveTexture(GL_TEXTURE0);
}