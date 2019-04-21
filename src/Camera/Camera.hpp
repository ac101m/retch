#ifndef _RETCH_CAMERA_INCLUDED
#define _RETCH_CAMERA_INCLUDED


#include <glm/glm.hpp>


namespace retch {

  class Camera {
    private:
      // View matrix parameters
      glm::vec3 up_cs;
      glm::vec3 right_cs;
      glm::vec3 forward_cs;
      glm::vec3 position;

      // Projection matrix parameters
      float fov;
      float aspectRatio;
      float minDistance;
      float maxDistance;

  //====[METHODS]==============================================================//

      void UpdateProjMat(void);
      void UpdateViewMat(void);

    public:
      // matrices
      glm::mat4 projMat;
      glm::mat4 viewMat;

  //====[METHODS]==============================================================//

      Camera(void);
  };

} // Namespace retch

#endif // _RETCH_CAMERA_INCLUDED
