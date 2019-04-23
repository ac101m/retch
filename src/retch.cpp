// standard
#include <iostream>
#include <stdlib.h>

// External
#include <optparse.hpp>

// Internal
#include "Utils/Buffer2D.hpp"
#include "Window/Window.hpp"
#include "Camera/Camera.hpp"
#include "Geometry/Mesh.hpp"
#include "Camera/Camera.hpp"
#include "Shader/Shader.hpp"


// Shader uniforms
typedef struct {
  glm::mat4 m;
  glm::mat4 mvp;
} Uniforms_t;


// Raster parameters
typedef struct {
  glm::vec3 nml_ws;    // Vertex normal in world space
} RParams_t;


// Vertex shader, returns position of vertex in homogenous coordinates
glm::vec4 VS(Uniforms_t& uniforms, retch::Vertex& vertex, RParams_t& vsOut) {

  // Pass normal directly for now
  vsOut.nml_ws = vertex.normal;

  // Return position of translated vertex
  return uniforms.mvp * glm::vec4(vertex.position, 1.0f);
}


// Fragment shader returns RGBA output
glm::vec4 FS(
  Uniforms_t& uniforms,
  RParams_t& fsIn) {

  return glm::vec4(fsIn.nml_ws, 1);
}


retch::Mesh GenTestMesh(void) {
  std::vector<retch::Vertex> vertices = {
    {glm::vec3(0, 0.5, 0),      glm::vec2(0, 0),  glm::vec3(1, 0, 0)},
    {glm::vec3(-0.5, -0.5, 0),  glm::vec2(0, 0),  glm::vec3(0, 1, 0)},
    {glm::vec3(0.5, -0.5, 0),   glm::vec2(0, 0),  glm::vec3(0, 0, 1)}};
  retch::Mesh mesh(vertices);
  return mesh;
}


// Add command line optioons to the option parser
void AddOptions(OptionParser& opt) {
  opt.Add(Option("displayx", 'x', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"640"}));
  opt.Add(Option("displayy", 'y', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"480"}));
  opt.Add(Option("drawratio", 'r', ARG_TYPE_FLOAT,
                 "Ratio of render to window size",
                 {"1.0"}));
}


int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple 3d renderer from scratch");
  AddOptions(opt);

  // Get display parameters and open our window
  int displayx = opt.Get("displayx");
  int displayy = opt.Get("displayy");
  retch::Window window(displayx, displayy, "retch");

  // Create raw pixel buffers for rendering
  float drawRatio = opt.Get("drawratio");
  int renderx = (float)displayx * drawRatio;
  int rendery = (float)displayy * drawRatio;
  retch::Buffer2D<glm::vec4> frameBuffer(renderx, rendery);
  retch::Buffer2D<float> depthBuffer(renderx, rendery);

  // Set up shader and camera
  retch::Shader<Uniforms_t, RParams_t> shader(VS, FS);
  Uniforms_t uniforms;
  uniforms.m = glm::mat4(1.0f);

  // Set up the camera
  retch::Camera camera;
  camera.SetViewRatio((float)renderx / (float)rendery);
  camera.SetPosition({0, 0, -2});

  // Generate a test mesh
  retch::Mesh mesh = GenTestMesh();

  // Draw loop
  while(!window.Done()) {
    frameBuffer.Clear(glm::vec4(0.0f));
    depthBuffer.Clear(1.0f);

    uniforms.m = glm::rotate(uniforms.m, (float)0.1, glm::vec3(0, 1, 0));
    uniforms.mvp = camera.projMat * camera.viewMat * uniforms.m;
    shader.Draw(frameBuffer, depthBuffer, uniforms, mesh);

    window.Output(frameBuffer);
    window.Refresh();
  }

  // El fin
  return 0;
}
