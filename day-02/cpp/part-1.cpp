#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("bad arguments");
  }
  auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("cannot open file");
  }


  auto valid_passwords{0};

  while (not infile.eof()) {
    int low, high;
    char letter, colon;
    std::string password;
    infile >> low >> high >> letter >> colon >> password;
    high *= -1;
    if (infile.eof()) { break; }

    auto letter_count{0};
    for (auto pw_letter : password)
      if (pw_letter == letter)
        ++letter_count;
    if (low <= letter_count and letter_count <= high)
      ++valid_passwords;
  }

  std::cout << valid_passwords << " passwords are valid."
            << std::endl;
  return EXIT_SUCCESS;
}

