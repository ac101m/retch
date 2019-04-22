#ifndef _RETCH_BUFFER_INCLUDED
#define _RETCH_BUFFER_INCLUDED

// Standard
#include <vector>


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

    // Overloaded access operator
    T*& operator[](int const i) {
      return this->index[i];
    }

    // Get raw element pointer
    std::vector<T>& data(void) {return this->dataVector;}

    // Width and height gets
    unsigned Width(void) {return this->width;}
    unsigned Height(void) {return this->height;}
  };

} // namespace retch

#endif // _RETCH_BUFFER_INCLUDED
