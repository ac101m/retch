#include "Camera/Camera.hpp"
using namespace retch;


// Void constructor, sets some sensible defaults
Camera::Camera(void) : up_cs(glm::vec3(0, 1, 0)),
                       right_cs(glm::vec3(1, 0, 0)),
                       forward_cs(glm::vec3(0, 0, 1)),
                       position(glm::vec3(0, 0, -2)) {

  // Set up view matrix parameters
  this->fov = 90.0;
  this->aspectRatio = 1.0;
  this->minDistance = 0.1;
  this->maxDistance = 1000;

  // Update view and projection matrices
  this->UpdateProjMat();
  this->UpdateViewMat();
}


void Camera::UpdateViewMat(void) {

}


void Camera::UpdateProjMat(void) {

}
