#include "Geometry/Mesh.hpp"
using namespace retch;

// Standard
#include <iostream>
#include <fstream>
#include <sstream>


// Load mesh from path
Mesh::Mesh(std::string const path) {
  std::ifstream fs(path);
  std::string line;

  // Iterate over lines in file
  while(std::getline(fs, line)) {
    if(line.size() > 0) {
      std::stringstream ss(&line[1]);

      Vertex v;
      unsigned idx0, idx1, idx2;

      switch(line[0]) {
        case 'v':     // This line represents a vertex
          ss >> v.position.x >> v.position.y >> v.position.z;
          this->vertices.push_back(v);
          break;

        case 'f':     // This line represents a triangle
          ss >> idx0 >> idx1 >> idx2;
          this->indices.push_back(idx2 - 1);
          this->indices.push_back(idx1 - 1);
          this->indices.push_back(idx0 - 1);
          break;

        default:      // Line identifier not recognised
          std::cout << "Error, '" << path;
          std::cout << "', line identifier '" << line[0];
          std::cout << "' not recognised\n";
          exit(1);
      }
    }
  }

  // Check continuity of indices
  for(unsigned i = 0; i < this->indices.size(); i++) {
    if(this->indices[i] >= this->vertices.size()) {
      std::cout << "Error, index out of bounds: ";
      std::cout << this->indices[i] << ":" << this->vertices.size() << "\n";
      exit(1);
    }
  }
}


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
