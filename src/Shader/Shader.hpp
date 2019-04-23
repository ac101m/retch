#ifndef _RETCH_SHADER_INCLUDED
#define _RETCH_SHADER_INCLUDED

// Standard
#include <algorithm>

// External
#include "glm/glm.hpp"

// Internal
#include "Geometry/Mesh.hpp"
#include "Utils/Buffer2D.hpp"


namespace retch {

  // Class describes shader pipeline and implements rasterisation
  template <class TUniforms, class TRasterParams>
  class Shader {
  private:
    // Function pointers to user defined shaders
    glm::vec4 (*const vs)(TUniforms&, Vertex&, TRasterParams&);
    glm::vec4 (*const fs)(TUniforms&, TRasterParams&);

//====[METHODS]==============================================================//

    // Convert render coordinates into framebuffer coordinates
    inline glm::uvec2 PixelIndex(glm::vec2& pos, glm::uvec2& bufferSize) {
      unsigned x = ((pos.x + 1) / 2) * (float)bufferSize.x;
      unsigned y = ((pos.y + 1) / 2) * (float)bufferSize.y;
      return glm::ivec2(x, y);
    }


    // Convert pixel position to render coordinates
    inline glm::vec2 PixelPosition(glm::uvec2& pos, glm::uvec2& bufferSize) {
      float x = (((float)pos.x / (float)bufferSize.x) * 2.0f) - 1.0f;
      float y = (((float)pos.y / (float)bufferSize.y) * 2.0f) - 1.0f;
      return glm::vec2(x, y);
    }


    bool edgeFunction(
      glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const& c) {
      return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
    }


    // Performs rasterisation for a single polygon
    void DrawPoly(
      Buffer2D<glm::vec4>& frameBuffer,
      Buffer2D<float>& depthBuffer,
      TUniforms& uniforms,
      glm::vec4* v,
      TRasterParams* rasterParams) {

      // Convert back to cartesian coordinates
      glm::vec3 v0 = glm::vec3(v[0]) / v[0].w;
      glm::vec3 v1 = glm::vec3(v[1]) / v[1].w;
      glm::vec3 v2 = glm::vec3(v[2]) / v[2].w;

      // Get minimum and maximum coordinates for triangle
      glm::vec2 min = glm::vec2(
        std::min(v0.x, std::min(v1.x, v2.x)),
        std::min(v0.y, std::min(v1.y, v2.y)));
      glm::vec2 max = glm::vec2(
        std::max(v0.x, std::max(v1.x, v2.x)),
        std::max(v0.y, std::max(v1.y, v2.y)));

      // Convert to screen coordinates
      glm::uvec2 fbSize = frameBuffer.Size();
      glm::uvec2 pxMin = PixelIndex(min, fbSize);
      glm::uvec2 pxMax = PixelIndex(max, fbSize);

      // Rasterisation operation
      for(unsigned j = pxMin.y; j <= pxMax.y; j++) {
        for(unsigned i = pxMin.x; i <= pxMax.x; i++) {
          glm::uvec2 pxCurrent = glm::uvec2(i, j);
          glm::vec2 px = PixelPosition(pxCurrent, fbSize);

          bool inside = true;
          inside &= edgeFunction(v0, v1, px);
          inside &= edgeFunction(v1, v2, px);
          inside &= edgeFunction(v2, v0, px);

          // Are we in bounds of the triangle?
          if(inside) {

            // Compute interpolation weights
            float Wv0 = (((v1.y-v2.y)*(px.x-v2.x))+((v2.x-v1.x)*(px.y-v2.y))) /
                        (((v1.y-v2.y)*(v0.x-v2.x))+((v2.x-v1.x)*(v0.y-v2.y)));
            float Wv1 = (((v2.y-v0.y)*(px.x-v2.x))+((v0.x-v2.x)*(px.y-v2.y))) /
                        (((v1.y-v2.y)*(v0.x-v2.x))+((v2.x-v1.x)*(v0.y-v2.y)));
            float Wv2 = 1.0f - (Wv0 + Wv1);

            // Interpolate z only
            float zFrag = (Wv0 * v0.z) + (Wv1 * v1.z) + (Wv2 * v2.z);

            // Early z optimisation
            if(zFrag < depthBuffer[j][i]) {
              depthBuffer[j][i] = zFrag;

              // Interpolate raster parameter block
              TRasterParams fsIn;
              float* fsInPtr = (float*)&fsIn;
              float* vsOut0 = (float*)&rasterParams[0];
              float* vsOut1 = (float*)&rasterParams[1];
              float* vsOut2 = (float*)&rasterParams[2];
              for(unsigned k = 0; k < sizeof(TRasterParams) / sizeof(float); k++) {
                fsInPtr[k] = (vsOut0[k] * Wv0) + (vsOut1[k] * Wv1) + (vsOut2[k] * Wv2);
              }

              // Call the fragment shader
              frameBuffer[j][i] = this->fs(uniforms, fsIn);
            }
          }
        }
      }
    }


  public:
    Shader(
      glm::vec4 (*const vs)(TUniforms&, Vertex&, TRasterParams&),
      glm::vec4 (*const fs)(TUniforms&, TRasterParams&)) :
      vs(vs), fs(fs) {}


    // Draw routine
    void Draw(
      Buffer2D<glm::vec4>& frameBuffer,
      Buffer2D<float>& depthBuffer,
      TUniforms& uniforms,
      Mesh& mesh) {

      // Run vertex shader on each vertex
      std::vector<glm::vec4> vertPositions(mesh.Size());
      std::vector<TRasterParams> rasterParams(mesh.Size());
      for(unsigned i = 0; i < mesh.Size(); i++) {
        vertPositions[i] = this->vs(uniforms, mesh[i], rasterParams[i]);
      }

      // Draw triangles
      for(unsigned i = 0; i < mesh.Size(); i += 3) {
        this->DrawPoly(
          frameBuffer, depthBuffer, uniforms,
          &vertPositions[i], &rasterParams[i]);
      }
    }
  };

} // namespace retch

#endif // _RETCH_SHADER_INCLUDED
