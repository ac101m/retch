#ifndef _RETCH_WINDOW_INCLUDED
#define _RETCH_WINDOW_INCLUDED

// standard
#include <string>
#include <memory>

// External
#include <GLT/Window.hpp>
#include <GLT/Mesh.hpp>

// Internal
#include "Utils/Buffer2D.hpp"


namespace retch {

  class Window {
    private:
      // Pointers required due to lack of
      std::shared_ptr<GLT::Window> gltWindow;       // Underlying opengl window
      std::shared_ptr<GLT::ShaderProgram> shader;   // Shader program for rendering
      std::shared_ptr<GLT::Texture> fbTexture;      // Frame buffer texture
      std::shared_ptr<GLT::Mesh> mesh;              // Mesh to render texture to

    public:
      Window(int const width, int const height, std::string const name);

      bool Done(void) {return this->gltWindow->ShouldClose();}
      void Refresh(void);

      int Width() {return this->gltWindow->GetWindowSize().x;}
      int Height() {return this->gltWindow->GetWindowSize().y;}

      void Output(Buffer2D<glm::vec4>& frameBuffer);
  };


} // namespace retch

#endif // _RETCH_WINDOW_INCLUDED
