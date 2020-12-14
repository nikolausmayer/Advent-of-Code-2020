#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
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

  std::map<unsigned long, unsigned long> memory;

  std::string bitmask;
  while (true) {
    std::string word;
    infile >> word;
    if (infile.eof()) break;

    if (word == "mask") {
      infile >> word;  /// " = "
      infile >> bitmask;
    } else {
      const unsigned long address = std::stoi(word.substr(4, word.size()-5));

      std::vector<unsigned long> address_pool{address};
      for (auto idx{0u}; idx < bitmask.size(); ++idx) {
        const auto letter{bitmask[idx]};
        switch (letter) {
          case '1': {
            for (auto &a : address_pool) {
              a |= (1ul << (35-idx));
            }
            break;
          }
          case 'X': {
            const auto count{address_pool.size()};
            for (auto jdx{0u}; jdx < count; ++jdx) {
              address_pool[jdx] &= ~(1ul << (35-idx));
              address_pool.push_back(address_pool[jdx] | (1ul << (35-idx)));
            }
            break;
          }
          default: break;
        }
      }

      infile >> word;
      unsigned long value;
      infile >> value;

      for (auto a : address_pool)
        memory[a] = value;
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

