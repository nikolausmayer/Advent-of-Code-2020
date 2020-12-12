#include <cstdlib>
#include <fstream>
#include <iostream>


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

  int x{0}, y{0};
  int wx{10}, wy{-1};

  while (true) {
    char action;
    int count;
    infile >> action >> count;
    if (infile.eof()) break;

    switch (action) {
      case 'N': { wy -= count; break; }
      case 'S': { wy += count; break; }
      case 'E': { wx += count; break; }
      case 'W': { wx -= count; break; }
      case 'L': { 
        for (auto i{0}; i < std::abs(count)/90; ++i) {
          auto tmp{wx}; wx = wy; wy = -tmp; 
        }
        break; 
      }
      case 'R': { 
        for (auto i{0}; i < std::abs(count)/90; ++i) {
          auto tmp{wx}; wx = -wy; wy = tmp; 
        }
        break; 
      }
      case 'F': { x += wx*count; y += wy*count; break; }
    }
  }

  std::cout << "Manhattan distance to starting point is "
            << std::abs(x) + std::abs(y)
            << std::endl;

  return EXIT_SUCCESS;
}

