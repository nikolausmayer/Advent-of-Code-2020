#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
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

  std::vector<std::queue<int>> decks;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) break;
    if (line.starts_with("Player")) {
      decks.push_back({});
      continue;
    }
    if (line == "")
      continue;
    decks.back().push(std::stoi(line));
  }

  while (decks[0].size() and decks[1].size()) {
    const int a{decks[0].front()}; decks[0].pop();
    const int b{decks[1].front()}; decks[1].pop();
    if (a > b) {
      decks[0].push(a); decks[0].push(b);
    } else { 
      decks[1].push(b); decks[1].push(a);
    }
  }

  auto& deck{decks[0].size() ? decks[0] : decks[1]};
  auto multiplier{deck.size()};
  auto sum{0};
  while (deck.size()) {
    sum += multiplier * deck.front();
    deck.pop();
    --multiplier;
  }

  std::cout << "Winning player's score is " << sum << std::endl;

  return EXIT_SUCCESS;
}

