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

  int x{0}, y{0}, heading{0};

  while (true) {
    char action;
    int count;
    infile >> action >> count;
    if (infile.eof()) break;

    switch (action) {
      case 'N': { y -= count; break; }
      case 'S': { y += count; break; }
      case 'E': { x += count; break; }
      case 'W': { x -= count; break; }
      case 'L': { heading = (heading+count+360)%360; break; }
      case 'R': { heading = (heading-count+360)%360; break; }
      case 'F': {
        switch (heading) {
          case   0: { x += count; break; }
          case  90: { y -= count; break; }
          case 180: { x -= count; break; }
          case 270: { y += count; break; }
        }
        break;
      }
    }
  }

  std::cout << "Manhattan distance to starting point is "
            << std::abs(x) + std::abs(y)
            << std::endl;

  return EXIT_SUCCESS;
}

