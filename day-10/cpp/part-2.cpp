#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

  std::vector<int> adapters;
  /// Outlet joltage
  adapters.push_back(0);
  while (true) {
    int joltage;
    infile >> joltage;
    if (infile.eof()) break;
    adapters.push_back(joltage);
  }
  std::sort(std::begin(adapters), std::end(adapters));
  /// Device joltage
  adapters.push_back(adapters.back()+3);

  std::vector<unsigned long long> memory(adapters.size()-1);
  /// A single combination for "no adapter"
  memory[0] = 1;
  for (auto idx{1}; idx < (int)adapters.size()-1; ++idx) {
    auto combinations{0ull};
    /// Fibonacci-like sum construction
    for (auto jdx{-3}; jdx <= -1; ++jdx)
      if (idx+jdx >= 0)
        if (adapters[idx] - adapters[idx+jdx] <= 3)
          combinations += memory[idx+jdx];
    memory[idx] = combinations;
  }

  std::cout << "The adapters can be arranged in "
            << memory.back() << " different ways."
            << std::endl;

  return EXIT_SUCCESS;
}

