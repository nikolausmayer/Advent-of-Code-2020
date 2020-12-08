#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, std::vector<std::pair<int, std::string>>> bags_in;

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
    const std::string key{a+b};

    if (no == "no") {
      std::getline(infile, no);
      continue;
    }

    while (true) {
      infile >> a >> b >> bags;
      const std::string value{a+b};
      bags_in[key].push_back({std::stoi(no), value});
      if (bags[bags.size()-1] == '.')
        break;
      infile >> no;
    }
  }

  std::list<std::pair<int, std::string>> queue;
  queue.push_back({1, "shinygold"});
  auto total_bag_counter{0ull};
  while (queue.size()) {
    const auto& [num_outer, outer]{queue.front()};
    for (const auto& [num_inner, inner] : bags_in[outer]) {
      total_bag_counter += num_outer*num_inner;
      queue.push_back({num_outer*num_inner, inner});
    }
    queue.pop_front();
  }

  std::cout << total_bag_counter << " individual bags are required inside a single shiny gold bag."
            << std::endl;

  return EXIT_SUCCESS;
}

