#ifndef _RETCH_MESH_INCLUDED
#define _RETCH_MESH_INCLUDED

// Standard
#include <vector>
#include <string>

// Internal
#include <Geometry/Vertex.hpp>


namespace retch {

  class Mesh {
  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

//====[METHODS]==============================================================//

    // Combines two vertices
    void MergeVertices(Vertex& v0, Vertex& v1);

  public:
    Mesh(std::string const path);
    Mesh(std::vector<Vertex> const vertices);
    Mesh(std::vector<Vertex> const vertices,
         std::vector<unsigned> const indices);

    // Mesh manipulations
    void PruneVertices(void);
    void ComputeNormals(void);
    void ComputeTangentSpaceBasis(void);
    void MergeVertices(
      float const naThreshold,    // Normal angle threshold
      float const dpThreshold,    // Position delta threshold
      float const uvThreshold);   // UV coordinate delta threshold

    // Access operators (so we can treat this like an std::vector)
    Vertex& operator[](int const i) {return vertices[indices[i]];}
    unsigned Size(void) const {return this->indices.size();}
  };

} // namespace retch

#endif // _RETCH_MESH_INCLUDED
