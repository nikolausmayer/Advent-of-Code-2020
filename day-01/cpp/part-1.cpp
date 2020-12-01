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

  std::vector<int> entries;

  while (not infile.eof()) {
    std::string entry;
    infile >> entry;
    if (infile.eof()) { break; }
    entries.push_back(std::atoi(entry.c_str()));
  }

  for (auto a : entries) {
    for (auto b : entries) {
      if (a + b == 2020) {
        std::cout << a << " * " << b << " = "
                  << a*b << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

}

