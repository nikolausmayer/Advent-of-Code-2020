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

  std::vector<unsigned long> elements;
  while (true) {
    unsigned long x;
    infile >> x;
    if (infile.eof()) break;
    elements.push_back(x);
  }

  const unsigned preamble{25};

  unsigned long secret;

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
      secret = sum;
      break;
    }
  }

  unsigned long first{0};
  unsigned long second{0};
  unsigned long sum{0};
  while (sum != secret) {
    if (sum < secret) {
      sum += elements[second];
      ++second;
    } else {
      sum -= elements[first];
      ++first;
    }
  }
  auto minmax = std::minmax_element(elements.begin()+first,
                                    elements.begin()+second);
  std::cout << *minmax.first << " + " << *minmax.second 
            << " = " << (*minmax.first + *minmax.second)
            << std::endl;


  return EXIT_SUCCESS;
}

