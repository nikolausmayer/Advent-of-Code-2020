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

  int earliest; infile >> earliest;
  std::vector<int> busses;
  int busID{0};
  while (true) {
    char c = infile.get();
    if (infile.eof()) break;
    switch (c) {
      case 'x':
      case ',':
      case '\n': {
        if (busID > 0)
          busses.push_back(busID);
        busID = 0;
        break;
      }
      default: 
        busID = busID*10 + (c - 48);
    }
  }

  auto minwait{1e8};
  auto minbus{0};
  for (auto bus : busses) {
    auto wait = bus - (earliest % bus);
    if (wait < minwait) {
      minwait = wait;
      minbus = bus;
    }
  }

  std::cout << minbus << " * " << minwait
            << " = " << minbus * minwait
            << std::endl;

  return EXIT_SUCCESS;
}

