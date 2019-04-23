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
  position(glm::vec3(0, 0, 0)) {

  // Set up view matrix parameters
  this->fov = 45.0f;
  this->viewRatio = 1.0f;
  this->zMin = 0.1f;
  this->zMax = 100.0f;

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


void Camera::SetViewRatio(float const ratio) {
  this->viewRatio = ratio;
  this->UpdateProjMat();
}


void Camera::SetPosition(glm::vec3 const position) {
  this->position = position;
  this->UpdateViewMat();
}
