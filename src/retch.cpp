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
  opt.Add(Option("displaywidth", 'w', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"512"}));
  opt.Add(Option("displayheight", 'h', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"512"}));
}


int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple 3d renderer from scratch");
  AddOptions(opt);

  // Get display parameters and open our window
  int width = opt.Get("displaywidth");
  int height = opt.Get("displayheight");
  retch::Window window(width, height, "retch");

  // Create raw pixel buffers for the renderer
  retch::Buffer2D<glm::vec4> frameBuffer(width, height);
  retch::Buffer2D<float> depthBuffer(width, height);

  // Set some random pixel colours (TEMPORARY, for testing)
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
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
