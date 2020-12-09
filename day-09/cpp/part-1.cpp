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

  std::vector<unsigned long> elements;
  while (true) {
    unsigned long x;
    infile >> x;
    if (infile.eof()) break;
    elements.push_back(x);
  }

  const unsigned preamble{25};

  for (auto idx{preamble}; idx < elements.size(); ++idx) {
    auto sum{elements[idx]};
    bool found{false};
    for (auto a{idx-preamble}; a < idx and not found; ++a) {
      for (auto b{a+1}; b < idx and not found; ++b) {
        if (elements[a] != elements[b] and
            sum == elements[a] + elements[b])
          found = true;
      }
    }
    if (not found) {
      std::cout << "First nonconforming number is "
                << sum
                << std::endl;
      break;
    }
  }


  return EXIT_SUCCESS;
}

