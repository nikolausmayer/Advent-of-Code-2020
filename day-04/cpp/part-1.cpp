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

  auto valid_passports{0};

  bool byr{false};
  bool iyr{false};
  bool eyr{false};
  bool hgt{false};
  bool hcl{false};
  bool ecl{false};
  bool pid{false};
  bool cid{false};

  while (true) {
    std::string line;
    std::getline(infile, line, '\n');
    if (line == "") {
      if (byr and iyr and eyr and hgt and 
          hcl and ecl and pid) {
        ++valid_passports;
      }
      byr = false;
      iyr = false;
      eyr = false;
      hgt = false;
      hcl = false;
      ecl = false;
      pid = false;
      cid = false;
    }
    if (infile.eof()) break;
    std::istringstream iss{line};
    while (true) {
      std::string field, content;
      if (not std::getline(iss, field, ':')) break;
      std::getline(iss, content, ' ');
      field == "byr" ? byr = true :
      field == "iyr" ? iyr = true :
      field == "eyr" ? eyr = true :
      field == "hgt" ? hgt = true :
      field == "hcl" ? hcl = true :
      field == "ecl" ? ecl = true :
      field == "pid" ? pid = true :
      field == "cid" ? cid = true : 0;
    }
  }

  std::cout << valid_passports << " passports are \"valid\""
            << std::endl;

  return EXIT_SUCCESS;
}

