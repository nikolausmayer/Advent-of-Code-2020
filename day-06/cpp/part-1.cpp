#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
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

  auto total_answers{0};

  std::set<char> group_answers;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) break;

    if (line == "") {
      total_answers += group_answers.size();
      group_answers.clear();
      continue;
    }

    for (auto letter : line)
      group_answers.insert(letter);
  }
  total_answers += group_answers.size();
  group_answers.clear();

  std::cout << "Sum of counts = " << total_answers << std::endl;

  return EXIT_SUCCESS;
}

