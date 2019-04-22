#ifndef _RETCH_MESH_INCLUDED
#define _RETCH_MESH_INCLUDED

// Standard
#include <vector>
#include <string>

// Internal
#include <Geometry/Vertex.hpp>


namespace retch {

  class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

//====[METHODS]==============================================================//

    Mesh(std::vector<Vertex> const vertices);
    Mesh(std::vector<Vertex> const vertices,
         std::vector<unsigned> const indices);

    // Mesh manipulations
    void ComputeNormals(void);
    void ComputeTangentSpaceBasis(void);

    // Access operators (so we can treat this like an std::vector)
    inline Vertex& operator[](int i) {return vertices[indices[i]];}
    inline unsigned size(void) {return this->indices.size();}
  };

} // namespace retch

#endif // _RETCH_MESH_INCLUDED
