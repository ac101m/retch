// standard
#include <iostream>
#include <stdlib.h>

// External
#include <optparse.hpp>

// Internal
#include "Utils/Buffer2D.hpp"
#include "Window/Window.hpp"


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

  // Create raw pixel buffers for the renderer
  float drawRatio = opt.Get("drawratio");
  int renderx = (float)displayx * drawRatio;
  int rendery = (float)displayy * drawRatio;
  retch::Buffer2D<glm::vec4> frameBuffer(renderx, rendery);
  retch::Buffer2D<float> depthBuffer(renderx, rendery);

  // Set some random pixel colours (TEMPORARY, for testing)
  for(int i = 0; i < renderx; i++) {
    for(int j = 0; j < rendery; j++) {
      frameBuffer[j][i].x = (float)(rand() % 255) / 255;
      frameBuffer[j][i].y = (float)(rand() % 255) / 255;
      frameBuffer[j][i].z = (float)(rand() % 255) / 255;
    }
  }

  // Draw loop
  while(!window.Done()) {
    window.Output(frameBuffer);
    window.Refresh();
  }

  // El fin
  return 0;
}
