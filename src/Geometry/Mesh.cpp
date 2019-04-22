#include "Geometry/Mesh.hpp"
using namespace retch;

// Standard
#include <iostream>


// Unindexed list of vertices
Mesh::Mesh(
  std::vector<Vertex> const vertices) :
  vertices(vertices), indices(std::vector<unsigned>(vertices.size())) {

  // Assume vertices are grouped into triangles by order in list
  for(unsigned i = 0; i < this->vertices.size(); i++) {
    this->indices[i] = i;
  }
}


// Indexed list of vertices
Mesh::Mesh(
  std::vector<Vertex> const vertices,
  std::vector<unsigned> const indices) :
  vertices(vertices), indices(indices) {

  // Ensure that indices point at valid vertices
  for(unsigned i = 0; i < this->vertices.size(); i++) {
    if(this->indices[i] >= this->vertices.size()) {
      std::cout << "Error, mesh index out of bounds\n";
      exit(1);
    }
  }
}


// Compute normals automatically
void Mesh::ComputeNormals(void) {

  // Clear normals
  for(unsigned i = 0; i < this->vertices.size(); i++) {
    this->vertices[i].normal = glm::vec3(0, 0, 0);
  }

  for(unsigned i = 0; i < this->indices.size(); i += 3) {
    Vertex* v[3] = {&(*this)[i], &(*this)[i + 1], &(*this)[i + 2]};

    glm::vec3 faceNormal = glm::cross(
      v[1]->position - v[0]->position,
      v[2]->position - v[0]->position);

    for(unsigned j = 0; j < 3; j++) {
      v[j]->normal += glm::normalize(faceNormal);
    }
  }

  // Normalise vertex normals
  for(unsigned i = 0; i < this->vertices.size(); i++) {
    this->vertices[i].normal = glm::normalize(this->vertices[i].normal);
  }
}
