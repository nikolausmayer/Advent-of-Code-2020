#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>


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

  std::map<int, unsigned long> memory;

  auto onesmask{0ul};
  auto zeromask{0ul};
  while (true) {
    std::string word;
    infile >> word;
    if (infile.eof()) break;

    if (word == "mask") {
      onesmask = 0ul;
      zeromask = 0ul;
      infile >> word;  /// " = "
      infile >> word;  /// bitmask
      for (auto letter : word) {
        onesmask <<= 1;
        zeromask <<= 1;
        switch (letter) {
          case '0': { ++zeromask; break; }
          case '1': { ++onesmask; break; }
          default: break;
        }
      }
    } else {
      const int address = std::stoi(word.substr(4, word.size()-5));
      infile >> word;
      unsigned long value;
      infile >> value;

      value |= onesmask;
      value = ~(~value | zeromask);
      memory[address] = value;
    }
  }

  auto sum{0ull};
  for (auto [k, v] : memory)
    sum += v;

  std::cout << "Sum of all values in memory: "
            << sum
            << std::endl;

  return EXIT_SUCCESS;
}

