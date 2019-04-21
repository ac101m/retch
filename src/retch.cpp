// standard
#include <iostream>

// External
#include <optparse.hpp>
#include <GLT/Window.hpp>

// Internal
#include "Utils/Buffer2D.hpp"


// Add command line optioons to the option parser
void AddOptions(OptionParser& opt) {
  opt.Add(Option("displaywidth", 'w', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"480"}));
  opt.Add(Option("displayheight", 'h', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"320"}));
}


// Draw a pixel buffer to the display
void DrawBuffer(GLT::Window& window, retch::Buffer2D<glm::vec3>& pixels) {

}


int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple renderer from scratch");
  AddOptions(opt);

  // Get display parameters and open our window
  int width = opt.Get("displaywidth");
  int height = opt.Get("displayheight");
  GLT::Window window(width, height, "retch");
  window.EnableFpsCounter();

  // Create raw pixel buffers for the software renderer
  retch::Buffer2D<glm::vec4> frameBuffer(width, height);
  retch::Buffer2D<float> depthBuffer(width, height);

  while(!window.ShouldClose()) {
    DrawBuffer(window, frameBuffer);
    window.Refresh();
  }

  // El fin
  return 0;
}
