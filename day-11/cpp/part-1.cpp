#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


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

  /// Map is padded with a ring of '.' empty spots to 
  /// make iterating later easier.
  std::vector<std::string> map;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) break;
    if (map.size() == 0)
      map.emplace_back(std::string(line.size()+2, '.'));
    map.emplace_back("." + line + ".");
  }
  map.emplace_back(std::string(map.back().size(), '.'));


  const auto W{map[0].size()};
  const auto H{map.size()};

  while (true) {
    auto changes{0u};
    auto mapcopy{map};

    for (auto y{1u}; y < H-1; ++y) {
      for (auto x{1u}; x < W-1; ++x) {
        auto occupied_neighbors{(map[y-1][x-1] == '#') +
                                (map[y-1][x+0] == '#') +
                                (map[y-1][x+1] == '#') +
                                (map[y+0][x-1] == '#') +
                                (map[y+0][x+1] == '#') +
                                (map[y+1][x-1] == '#') +
                                (map[y+1][x+0] == '#') +
                                (map[y+1][x+1] == '#')};
        if      (map[y][x] == 'L' and 
                 occupied_neighbors == 0) {
          mapcopy[y][x] = '#';
          ++changes;
        } else if (map[y][x] == '#' and 
                 occupied_neighbors >= 4) {
          mapcopy[y][x] = 'L';
          ++changes;
        }
      }
    }

    if (changes == 0)
      break;

    map = mapcopy;
  }

  auto final_occupied{0u};
  for (auto y{1u}; y < H-1; ++y)
    for (auto x{1u}; x < W-1; ++x)
      final_occupied += (map[y][x] == '#');

  std::cout << final_occupied << " seats end up occupied."
            << std::endl;

  return EXIT_SUCCESS;
}

