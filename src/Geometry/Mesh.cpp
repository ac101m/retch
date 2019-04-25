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


// Remove vertices not pointed at by any indices
void Mesh::PruneVertices(void) {
  for(unsigned i = 0; i < this->indices.size(); i++) {

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
      v[0]->position - v[2]->position);

    for(unsigned j = 0; j < 3; j++) {
      v[j]->normal += glm::normalize(faceNormal);
    }
  }

  // Normalise vertex normals
  for(unsigned i = 0; i < this->vertices.size(); i++) {
    this->vertices[i].normal = glm::normalize(this->vertices[i].normal);
  }
}


// Merge two individual vertices
void Mesh::MergeVertices(Vertex& v0, Vertex& v1) {
  v0.position = (v0.position + v1.position) / 2.0f;
  v0.uv = (v0.uv + v1.uv) / 2.0f;
  v0.normal = glm::normalize(v0.normal + v1.normal);
  v0.tangent =  glm::normalize(v0.tangent + v1.tangent);
  v0.bitangent =  glm::normalize(v0.bitangent + v1.bitangent);
  v1 = v0;
}


// Merge like vertices based on thresholds
void Mesh::MergeVertices(
  float const naThreshold,
  float const dpThreshold,
  float const uvThreshold) {

  for(unsigned i = 0; i < this->vertices.size(); i++) {
    for(unsigned j = 0; j < i; j++) {
      float na = glm::acos(glm::dot(
        this->vertices[i].normal, this->vertices[j].normal));
      float dp = glm::length(
        this->vertices[i].position - this->vertices[j].position);
      float duv = glm::length(
        this->vertices[i].uv - this->vertices[j].uv);

      // If thresholds are met, merge the vertices
      if(dp <= dpThreshold && na <= naThreshold && duv <= uvThreshold) {
        this->MergeVertices(this->vertices[i], this->vertices[j]);
      }
    }
  }
}
