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

  auto one_difference{0};
  auto three_difference{0};

  for (auto idx{0u}; idx < adapters.size(); ++idx) {
    auto a{adapters[idx]};
    auto b{adapters[idx+1]};
    if (b-a == 1) 
      ++one_difference;
    if (b-a == 3)
      ++three_difference;
  }

  std::cout << one_difference << " * " << three_difference
            << " = " << one_difference * three_difference
            << std::endl;

  return EXIT_SUCCESS;
}

