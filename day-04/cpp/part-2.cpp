#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool valid_byr(const std::string& s) {
  if (s.size() != 4) return false;
  for (auto c : s) if (c < '0' or c > '9') return false;
  const int i{std::stoi(s)};
  if (i < 1920 or i > 2002) return false;
  return true;
}

bool valid_iyr(const std::string& s) {
  if (s.size() != 4) return false;
  for (auto c : s) if (c < '0' or c > '9') return false;
  const int i{std::stoi(s)};
  if (i < 2010 or i > 2020) return false;
  return true;
}

bool valid_eyr(const std::string& s) {
  if (s.size() != 4) return false;
  for (auto c : s) if (c < '0' or c > '9') return false;
  const int i{std::stoi(s)};
  if (i < 2020 or i > 2030) return false;
  return true;
}

bool valid_hgt(const std::string& s) {
  auto idx{0u};
  for (; idx < s.size(); ++idx)
    if (s[idx] < '0' or s[idx] > '9')
      break;
  const auto a{std::stoi(s.substr(0, idx))};
  if (idx == s.size()) return false;
  const auto b{s.substr(idx)};
  if (b == "cm") {
    if (a < 150 or a > 193)
      return false;
  } else if (b == "in") {
    if (a < 59 or a > 76)
      return false;
  } else {
    return false;
  }

  return true;
}

bool valid_hcl(const std::string& s) {
  if (s.size() != 7) return false;
  if (s[0] != '#') return false;
  for (auto idx{1u}; idx < s.size(); ++idx)
    if (not ((s[idx] >= '0' and s[idx] <= '9') or
             (s[idx] >= 'a' and s[idx] <= 'f')))
      return false;
          
  return true;
}

bool valid_ecl(const std::string& s) {
  return (s == "amb" or
          s == "blu" or
          s == "brn" or
          s == "gry" or
          s == "grn" or
          s == "hzl" or
          s == "oth");
}

bool valid_pid(const std::string& s) {
  if (s.size() != 9) return false;
  for (auto c : s)
    if (c < '0' or c > '9')
      return false;
  return true;
}



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
      field == "byr" ? byr = valid_byr(content) :
      field == "iyr" ? iyr = valid_iyr(content) :
      field == "eyr" ? eyr = valid_eyr(content) :
      field == "hgt" ? hgt = valid_hgt(content) :
      field == "hcl" ? hcl = valid_hcl(content) :
      field == "ecl" ? ecl = valid_ecl(content) :
      field == "pid" ? pid = valid_pid(content) :
      field == "cid" ? cid = true : 0;
    }
  }

  std::cout << valid_passports << " passports are \"valid\""
            << std::endl;

  return EXIT_SUCCESS;
}

