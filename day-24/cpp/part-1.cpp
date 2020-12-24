#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>


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

  std::map<std::pair<int, int>, bool> grid;

  
  int x{0}, y{0};
  while (true) {
    const int c{infile.get()};
    if (infile.eof()) break;
    std::cout << "--" << (char)c << std::endl;
    switch (c) {
      case '\n': {
        std::cout << x << " " << y << std::endl;
        grid[{x, y}] = !(grid[{x, y}]);
        x = 0; y = 0;
        break;
      }
      case 'w': {
        std::cout << "w" << std::endl;
        --x;
        break;
      }
      case 'e': {
        std::cout << "e" << std::endl;
        ++x;
        break;
      }
      case 'n': {
        const int c2{infile.get()};
        if (y % 2 == 0) {
          if (c2 == 'w') {
            std::cout << "nw" << std::endl;
            --x;
          } else {
            std::cout << "ne" << std::endl;
          }
        } else {
          if (c2 == 'w') {
            std::cout << "nw" << std::endl;
          } else {
            std::cout << "ne" << std::endl;
            ++x;
          }
        }
        --y;
        break;
      }
      case 's': {
        const int c2{infile.get()};
        if (y % 2 == 0) {
          if (c2 == 'w') {
            std::cout << "sw" << std::endl;
            --x;
          } else {
            std::cout << "se" << std::endl;
          }
        } else {
          if (c2 == 'w') {
            std::cout << "sw" << std::endl;
          } else {
            std::cout << "se" << std::endl;
            ++x;
          }
        }
        ++y;
        break;
      }
    }
  }

  auto black_tiles{0u};
  for (const auto& [k, v] : grid)
    black_tiles += v;

  std::cout << black_tiles << " are left black side up." 
            << std::endl;


  return EXIT_SUCCESS;
}

