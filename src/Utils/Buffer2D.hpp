#ifndef _RETCH_BUFFER_INCLUDED
#define _RETCH_BUFFER_INCLUDED


#include <vector>


namespace retch {

  template<class T>
  class Buffer2D {
    private:
      // Buffer data storage
      std::vector<T> data;
      std::vector<T*> index;

    public:
      Buffer2D(int const width, int const height) {
        this->data = std::vector<T>(width * height);
        this->index = std::vector<T*>(height);

        for(unsigned i = 0; i < this->index.size(); i++) {
          this->index[i] = &this->data[i * width];
        }
      }

      // Overload access operator
      T*& operator[](int const i) {
        return this->index[i];
      }
  };

} // namespace retch

#endif // _RETCH_BUFFER_INCLUDED
