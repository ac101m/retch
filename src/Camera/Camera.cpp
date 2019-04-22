#include "Camera/Camera.hpp"
using namespace retch;

// External
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


// Void constructor, sets some sensible defaults
Camera::Camera(void) :
  up(glm::vec3(0, 1, 0)),
  right(glm::vec3(1, 0, 0)),
  forward(glm::vec3(0, 0, 1)),
  position(glm::vec3(0, 0, -2)) {

  // Set up view matrix parameters
  this->fov = 90.0;
  this->viewRatio = 1.0;
  this->zMin = 0.1;
  this->zMax = 1000;

  // Update view and projection matrices
  this->UpdateProjMat();
  this->UpdateViewMat();
}


void Camera::UpdateViewMat(void) {
  this->viewMat = glm::lookAt(
    this->position,
    this->position + this->forward,
    this->up);
}


void Camera::UpdateProjMat(void) {
  this->projMat = glm::perspective(
    this->fov, this->viewRatio,
    this->zMin, this->zMax);
}
