#ifndef _RETCH_BUFFER_INCLUDED
#define _RETCH_BUFFER_INCLUDED

// Standard
#include <vector>

// External
#include "glm/glm.hpp"


namespace retch {

  template<class T>
  class Buffer2D {
  private:
    unsigned width;
    unsigned height;

    // Buffer data storage
    std::vector<T> dataVector;
    std::vector<T*> index;

  public:
    Buffer2D(int const width, int const height) {
      this->width = width;
      this->height = height;

      this->dataVector = std::vector<T>(width * height);
      this->index = std::vector<T*>(height);

      for(unsigned i = 0; i < this->index.size(); i++) {
        this->index[i] = &this->dataVector[i * width];
      }
    }

    // Initialise the buffer
    void Clear(T initPattern) {
      for(unsigned i = 0; i < dataVector.size(); i++) {
        dataVector[i] = initPattern;
      }
    }

    // Overloaded access operator
    T*& operator[](int const i) {
      return this->index[i];
    }

    // Get raw element pointer
    std::vector<T>& data(void) {return this->dataVector;}

    // Width and height gets
    unsigned Width(void) {return this->width;}
    unsigned Height(void) {return this->height;}
    glm::uvec2 Size(void) {return glm::uvec2(this->width, this->height);}
  };

} // namespace retch

#endif // _RETCH_BUFFER_INCLUDED
