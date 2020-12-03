#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("bad arguments");
  }
  auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("cannot open file");
  }

  std::vector<std::string> map;

  while (not infile.eof()) {
    std::string line;
    infile >> line;
    if (infile.eof()) { break; }
    map.emplace_back(std::move(line));
  }

  auto dx{3}, dy{1};
  auto x{0u}, y{0u};
  auto line_width{map[0].size()};

  auto tree_counter{0};
  while (y < map.size()) {
    if (map[y][x%line_width] == '#') {
      ++tree_counter;
    }

    x += dx;
    y += dy;
  }

  std::cout << "Bumped into " << tree_counter << " trees."
            << std::endl;

  return EXIT_SUCCESS;
}

