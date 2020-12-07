#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

std::map<std::string, std::vector<std::string>> can_contain;

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

  while (true) {
    std::string a, b, bags, contain, no;
    infile >> a >> b >> bags >> contain >> no;
    if (infile.eof()) break;
    const std::string value{a+b};

    if (no == "no") {
      std::getline(infile, no);
      continue;
    }

    while (true) {
      infile >> a >> b >> bags;
      const std::string key{a+b};
      can_contain[key].push_back(value);
      if (bags[bags.size()-1] == '.')
        break;
      infile >> no;
    }
  }

  std::set<std::string> result;
  result.insert("shinygold");
  while (true) {
    auto set_copy{result};
    for (const auto& containee: result)
      for (const auto& container: can_contain[containee])
        set_copy.insert(container);
    if (set_copy == result)
      break;
    result = set_copy;
  }

  std::cout << result.size()-1 << " bags can contain shiny gold."
            << std::endl;


  return EXIT_SUCCESS;
}

