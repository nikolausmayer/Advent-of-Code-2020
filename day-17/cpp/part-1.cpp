#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("bad arguments");
  }
  const auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("cannot open file");
  }

  constexpr unsigned PAD{7};
  auto dim{0u};
  auto size{0u};

  std::uint8_t* space{nullptr};

  {
    auto idx{0u};
    while (true) {
      std::string line;
      infile >> line;
      if (infile.eof()) break;
      if (not space) {
        dim = line.size()+PAD+PAD;
        size = dim*dim*(PAD+PAD+1);
        space = new std::uint8_t[size];
        std::fill(space, space+size, 0);
        idx += dim*dim*PAD+dim*PAD+PAD;
      }
      for (auto c : line) {
        switch (c) {
          case '#': { space[idx++] = 1; break; }
          case '.': { space[idx++] = 0; break; }
        }
      }
      idx += dim - line.size();
    }
  }

  auto IDX = [dim](unsigned x, unsigned y, unsigned z) {
    return dim*dim*z + dim*y + x;
  };

  for (auto iter{0}; iter < 6; ++iter) {
    /// X pass
    std::uint8_t* X{new std::uint8_t[size]};
    std::fill(X, X+size, 0);
    for (auto z{1u}; z < (PAD+PAD+1)-1; ++z)
      for (auto y{1u}; y < dim-1; ++y)
        for (auto x{1u}; x < dim-1; ++x)
          X[IDX(x,y,z)] = space[IDX(x-1,y,z)] + 
                          space[IDX(x  ,y,z)] +
                          space[IDX(x+1,y,z)];
    /// Y pass
    std::uint8_t* XY{new std::uint8_t[size]};
    std::fill(XY, XY+size, 0);
    for (auto z{1u}; z < (PAD+PAD+1)-1; ++z)
      for (auto y{1u}; y < dim-1; ++y)
        for (auto x{1u}; x < dim-1; ++x)
          XY[IDX(x,y,z)] = X[IDX(x,y-1,z)] + 
                           X[IDX(x,y  ,z)] +
                           X[IDX(x,y+1,z)];

    /// XYZ pass
    std::uint8_t* XYZ{new std::uint8_t[size]};
    std::fill(XYZ, XYZ+size, 0);
    for (auto z{1u}; z < (PAD+PAD+1)-1; ++z)
      for (auto y{1u}; y < dim-1; ++y)
        for (auto x{1u}; x < dim-1; ++x)
          XYZ[IDX(x,y,z)] = XY[IDX(x,y,z-1)] + 
                            XY[IDX(x,y,z  )] +
                            XY[IDX(x,y,z+1)];

    for (auto z{1u}; z < (PAD+PAD+1)-1; ++z)
      for (auto y{1u}; y < dim-1; ++y)
        for (auto x{1u}; x < dim-1; ++x)
          if (space[IDX(x,y,z)] == 1) {
            if (XYZ[IDX(x,y,z)]-1 != 2 and
                XYZ[IDX(x,y,z)]-1 != 3) {
              space[IDX(x,y,z)] = 0;
            }
          } else {
            if (XYZ[IDX(x,y,z)] == 3) {
              space[IDX(x,y,z)] = 1;
            }
          }

    delete[] XYZ;
    delete[] XY;
    delete[] X;
  }

  auto active{0u};
  for (auto z{0u}; z < (PAD+PAD+1); ++z)
    for (auto y{0u}; y < dim; ++y)
      for (auto x{0u}; x < dim; ++x)
        active += space[IDX(x,y,z)];
  std::cout << active << " cubes are active after 6 cycles."
            << std::endl;


  delete[] space;

  return EXIT_SUCCESS;
}

