// standard
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
using namespace std::chrono;

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
  glm::mat4 mMx;
  glm::mat4 mvpMx;
  glm::vec3 eyePos_ws;      // Position of camera in world space
  glm::vec3 aLightPow;      // Ambient light power
  glm::vec3 pLightPow;      // Point light power
  glm::vec3 pLightPos_ws;   // Point light position in worldspace
  glm::vec3 objectColour;
} Uniforms_t;


// Raster parameters
typedef struct {
  glm::vec3 pos_ws;         // Position in world space
  glm::vec3 nml_ws;         // Normal in world space
} RParams_t;


// Vertex shader, returns position of vertex in homogenous coordinates
glm::vec4 VS(Uniforms_t& uniforms, retch::Vertex& vertex, RParams_t& vsOut) {
  vsOut.pos_ws = uniforms.mMx * glm::vec4(vertex.position, 1.0f);
  vsOut.nml_ws = uniforms.mMx * glm::vec4(vertex.normal, 1.0f);
  return uniforms.mvpMx * glm::vec4(vertex.position, 1.0f);
}


// Fragment shader returns RGBA output
glm::vec4 FS(Uniforms_t& uniforms,RParams_t& fsIn) {

  // Ambient component
  glm::vec3 lAmbient = uniforms.aLightPow;

  // Diffuse component
  glm::vec3 dir = uniforms.pLightPos_ws - fsIn.pos_ws;
  float d2 = (dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z);
  float angle = std::max(glm::dot(fsIn.nml_ws, glm::normalize(dir)), 0.0f);
  glm::vec3 lDiffuse = angle * (uniforms.pLightPow / d2);

  // Specular component
  glm::vec3 eyeDir = glm::normalize(uniforms.eyePos_ws - fsIn.pos_ws);
  glm::vec3 reflectDir = glm::reflect(-glm::normalize(dir), fsIn.nml_ws);
  float spec = glm::pow(glm::max(glm::dot(eyeDir, reflectDir), 0.0f), 64.0f);
  glm::vec3 lSpecular = spec * (uniforms.pLightPow / d2);

  // Add up lighting components
  glm::vec3 lTotal = (lAmbient + lDiffuse + lSpecular) * uniforms.objectColour;
  return glm::vec4(lTotal, 1.0f);
}


// Simple single triangle test mesh
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
  uniforms.mMx = glm::mat4(1);
  uniforms.aLightPow = glm::vec3(0.1);
  uniforms.pLightPow = glm::vec3(12);
  uniforms.pLightPos_ws = glm::vec3(-2, 3, -5);
  uniforms.objectColour = glm::vec3(1, 1, 1);

  // Set up the camera
  retch::Camera camera;
  camera.SetViewRatio((float)renderx / (float)rendery);
  camera.SetPosition(glm::vec3(0, 1.5, -6));

  // Generate test meshes
  retch::Mesh triangle = GenTestMesh();
  retch::Mesh teapot("data/Objects/Teapot.obj");
  teapot.ComputeNormals();
  teapot.MergeVertices(1.0, 0.01, 0);

  // Timing variables
  float tClear = 0.0f;
  float tDraw = 0.0f;
  float tOutput = 0.0f;
  float tExpAvg = 0.99f;

  // Draw loop
  while(!window.Done()) {
    steady_clock::time_point tStart = steady_clock::now();

    // Clear render buffers
    frameBuffer.Clear(glm::vec4(0.0f));
    depthBuffer.Clear(1.0f);
    tClear = tClear * tExpAvg + (1.0f - tExpAvg) *
      duration_cast<microseconds>(steady_clock::now() - tStart).count() / 1000.0f;

    // Set uniforms and make draw calls
    uniforms.eyePos_ws = camera.GetPosition();
    uniforms.mMx = glm::rotate(uniforms.mMx, (float)0.002, glm::vec3(0, 1, 0));
    uniforms.mvpMx = camera.projMat * camera.viewMat * uniforms.mMx;
    shader.Draw(frameBuffer, depthBuffer, uniforms, teapot);
    tDraw = tDraw * tExpAvg + (1.0f - tExpAvg) *
      duration_cast<microseconds>(steady_clock::now() - tStart).count() / 1000.0f;

    // Display the frame buffer
    window.Output(frameBuffer);
    tOutput = tOutput * tExpAvg + (1.0f - tExpAvg) *
      duration_cast<microseconds>(steady_clock::now() - tStart).count() / 1000.0f;

    // Display timing information in the window title bar
    std::stringstream ss;
    ss << "retch: render (ms) [" << std::fixed << std::setprecision(2);
    ss << tClear << ", ";
    ss << tDraw - tClear << ", ";
    ss << tOutput - tDraw << "] [";
    ss << tOutput << "]";
    window.SetTitle(ss.str());
    window.Refresh();
  }

  // El fin
  return 0;
}
