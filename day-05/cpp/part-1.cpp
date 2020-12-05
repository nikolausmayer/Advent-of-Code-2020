#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
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

  auto highest_seatID{0};
  while (true) {
    std::string line;
    infile >> line;
    if (infile.eof()) break;

    std::uint8_t row{0}, col{0};
    for (auto i{0u}; i < 7; ++i)
      row = (row << 1) + (line[i] == 'F' ? 0 : 1);
    for (auto i{7u}; i < 10; ++i)
      col = (col << 1) + (line[i] == 'L' ? 0 : 1);
    auto seatID{8 * row + col};
    highest_seatID = std::max(highest_seatID, seatID);
  }
  std::cout << "Highest seat ID is " << highest_seatID 
            << std::endl;

  return EXIT_SUCCESS;
}

