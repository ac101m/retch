#ifndef _RETCH_CAMERA_INCLUDED
#define _RETCH_CAMERA_INCLUDED

// External
#include <glm/glm.hpp>


namespace retch {

  class Camera {
  private:
    // View matrix parameters
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
    glm::vec3 position;

    // Projection matrix parameters
    float fov;
    float viewRatio;
    float zMin;
    float zMax;

//====[METHODS]==============================================================//

    void UpdateProjMat(void);
    void UpdateViewMat(void);

  public:
    // matrices
    glm::mat4 projMat;
    glm::mat4 viewMat;

//====[METHODS]==============================================================//

    Camera(void);

    // Camera property sets
    void SetViewRatio(float const ratio);
    void SetPosition(glm::vec3 const position);

    // Property gets
    glm::vec3 GetPosition(void) {return this->position;}
  };

} // Namespace retch

#endif // _RETCH_CAMERA_INCLUDED
