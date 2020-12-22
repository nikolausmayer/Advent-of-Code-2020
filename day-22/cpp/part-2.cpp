#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

bool playGame (auto& decks, std::string prefix)
{
  std::map<std::string, bool> state_memory;

  while (decks[0].size() and decks[1].size()) {
    {
      std::ostringstream oss;
      for (auto n : decks[0])
        oss << n << "-";
      oss << "|";
      for (auto n : decks[1])
        oss << n << "-";

      if (state_memory[oss.str()]) {
        //std::cout << prefix << "Previous state encountered, Player 1 wins!\n";
        return false;
      }
      state_memory[oss.str()] = true;
    }

    const auto a{decks[0].front()}; decks[0].pop_front();
    const auto b{decks[1].front()}; decks[1].pop_front();
    //std::cout << "\n";
    //std::cout << prefix << "Player 1: " << a << ": ";
    //for (auto n : decks[0]) std::cout << n << " ";
    //std::cout << "\n";
    //std::cout << prefix << "Player 2: " << b << ": ";
    //for (auto n : decks[1]) std::cout << n << " ";
    //std::cout << "\n";
    if (a > decks[0].size() or b > decks[1].size()) {
      if (a > b) {
        //std::cout << prefix << "Player 1 wins (" << a << " vs " << b << ")\n";
        decks[0].push_back(a); decks[0].push_back(b);
      } else { 
        //std::cout << prefix << "Player 2 wins (" << a << " vs " << b << ")\n";
        decks[1].push_back(b); decks[1].push_back(a);
      }
    } else {
      //std::cout << prefix << "--recurse (" << a << "/" << decks[0].size() << " vs " << b << "/" << decks[1].size() << ")--\n";
      std::vector<std::list<unsigned>> newdecks{2};
      auto ca{a};
      for (auto e : decks[0]) { 
        newdecks[0].push_back(e);
        --ca;
        if (ca == 0) break;
      }
      auto cb{b};
      for (auto e : decks[1]) { 
        newdecks[1].push_back(e);
        --cb;
        if (cb == 0) break;
      }

      if (playGame(newdecks, prefix+"  ")) {
        decks[1].push_back(b); decks[1].push_back(a);
      } else {
        decks[0].push_back(a); decks[0].push_back(b);
      }
    }
  }

  return (decks[0].size() ? false : true);
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

  std::vector<std::list<unsigned>> decks;

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
    decks.back().push_back(std::stoi(line));
  }

  auto winner = playGame(decks, "");

  auto& deck{decks[winner]};
  auto multiplier{deck.size()};
  auto sum{0};
  while (deck.size()) {
    sum += multiplier * deck.front();
    deck.pop_front();
    --multiplier;
  }

  std::cout << "Winning player's score is " << sum << std::endl;

  return EXIT_SUCCESS;
}

