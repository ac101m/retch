#ifndef _RETCH_VERTEX_INCLUDED
#define _RETCH_VERTEX_INCLUDED

// External
#include <glm/glm.hpp>


namespace retch {

  // Class contains raw vertex data
  class Vertex {
  public:
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
  };

} // namespace retch

#endif // _RETCH_VERTEX_INCLUDED
