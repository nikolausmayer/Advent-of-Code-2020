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

  std::vector<int> numbers;

  while (true) {
    int i;
    infile >> i;
    if (infile.eof()) break;
    numbers.push_back(i);
    infile.get();
  }

  std::map<int, int> spoken_count;
  std::map<int, int> spoken_last;
  std::map<int, int> spoken_prelast;

  for (auto turn{0u}; turn < 2020; ++turn) {
    auto n{numbers[turn]};
    ++spoken_count[n];
    spoken_prelast[n] = spoken_last[n];
    spoken_last[n] = turn;
    if (turn == numbers.size()-1) [[likely]] {
      auto last{numbers.back()};
      if (spoken_count[last] == 1) {
        numbers.push_back(0);
      } else {
        numbers.push_back(spoken_last[last] - 
                          spoken_prelast[last]);
      }
    }
  }

  std::cout << "2020th number spoken is "
            << numbers[numbers.size()-2]
            << std::endl;


  return EXIT_SUCCESS;
}

