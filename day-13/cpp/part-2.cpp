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

  int dummy; infile >> dummy;
  std::vector<int> busses;
  int busID{0};
  while (true) {
    char c = infile.get();
    if (infile.eof()) break;
    switch (c) {
      case 'x': {
        busID = -1;
        break;
      }
      case ',':
      case '\n': {
        if (busID != 0)
          busses.push_back(busID);
        busID = 0;
        break;
      }
      default: 
        busID = busID*10 + (c - 48);
    }
  }

  
  auto start{0ull};
  auto period{0ull + busses[0]};
  for (auto idx{1u}; idx < busses.size(); ++idx) {
    if (busses[idx] < 0) continue;
    const int bus{busses[idx]};
    int remainder{bus - (int)idx};
    while (remainder < 0)
      remainder += bus;

    while (start % bus != (unsigned long long)remainder)
      start += period;
    period *= bus;
  }

  std::cout << "First departure point is at "
            << start
            << std::endl;

  return EXIT_SUCCESS;
}

