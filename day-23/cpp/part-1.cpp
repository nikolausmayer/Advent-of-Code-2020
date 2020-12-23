#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>


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

  std::list<int> cups;
  int mincup{10}, maxcup{0};
  while (true) {
    int c{infile.get()};
    if (c == '\n') break;
    c -= '0';
    cups.push_back(c);

    mincup = std::min(mincup, c);
    maxcup = std::max(maxcup, c);
  }

  auto current_cup{cups.begin()};
  auto iterplus = [](auto& iter, auto& list) {
    ++iter;
    if (iter == list.end())
      iter = list.begin();
  };
  
  for (auto move{0}; move < 100; ++move) {
    std::list<int> removed_cups;
    for (auto i{0}; i < 3; ++i) {
      auto copyiter{current_cup};
      iterplus(copyiter, cups);
      removed_cups.push_back(*copyiter);
      cups.erase(copyiter);
    }

    int destination_cup{*current_cup};
    while (true) {
      --destination_cup;
      if (destination_cup < mincup) destination_cup = maxcup;
      auto rmci{removed_cups.begin()};
      if (destination_cup == *rmci++) continue;
      if (destination_cup == *rmci++) continue;
      if (destination_cup == *rmci)   continue;
      break;
    }

    auto insert_before{cups.begin()};
    while (*insert_before != destination_cup) iterplus(insert_before, cups);
    iterplus(insert_before, cups);
    for (auto i{0}; i < 3; ++i) {
      cups.insert(insert_before, removed_cups.front());
      removed_cups.pop_front();
    }

    iterplus(current_cup, cups);
  }

  auto printiter{std::find(cups.begin(), cups.end(), 1)};
  iterplus(printiter, cups);
  for (auto i{0u}; i < cups.size()-1; ++i) {
    std::cout << *printiter;
    iterplus(printiter, cups);
  }
  std::cout << std::endl;


  return EXIT_SUCCESS;
}

