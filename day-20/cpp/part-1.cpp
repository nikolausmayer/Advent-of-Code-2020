#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>


class Tile
{
public:
  Tile() { }

  Tile(int ID)
    : m_ID(ID)
  { }

  void addLine(const std::string& line)
  {
    m_data.push_back(line);
  }

  std::vector<unsigned> getPossibleEdges() const
  {
    std::vector<unsigned> edges;
    { auto i{0u}; for (auto c : m_data[0]) i = 2*i+(c=='#'); edges.push_back(i); }
    { auto i{0u}; for (auto c : m_data[m_data.size()-1]) i = 2*i+(c=='#'); edges.push_back(i); }

    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[0][m_data.size()-1-idx]=='#'); edges.push_back(i); }
    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[m_data.size()-1][m_data.size()-1-idx]=='#'); edges.push_back(i); }

    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[idx].front()=='#'); edges.push_back(i); }
    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[m_data.size()-1-idx].front()=='#'); edges.push_back(i); }

    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[idx].back()=='#'); edges.push_back(i); }
    { auto i{0u}; for (auto idx{0u}; idx < m_data.size(); ++idx)  i = 2*i+(m_data[m_data.size()-1-idx].back()=='#'); edges.push_back(i); }
    return edges;
  }

private:
  int m_ID{0};
  std::vector<std::string> m_data;
};


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

  std::vector<int> IDs;
  std::map<int, Tile> tiles;

  while (true) {
    std::string word;
    int ID;
    infile >> word >> ID;
    if (infile.eof()) break;
    Tile tile{ID};
    std::getline(infile, word);

    while (true) {
      std::getline(infile, word);
      if (word == "") break;
      tile.addLine(word);
    }

    tiles[ID] = tile;
  }


  auto product_of_corner_IDs{1ull};

  for (const auto& [ID, tile] : tiles) {
    auto matching_edges{0};
    auto edges{tile.getPossibleEdges()};
    for (auto edge : edges) {
      for (const auto& [ID2, tile2] : tiles) {
        if (ID == ID2) continue;
        for (auto edge2 : tile2.getPossibleEdges()) {
          if (edge == edge2) {
            ++matching_edges;
            break;
          }
        }
      }
    }

    if (matching_edges == 4)
      product_of_corner_IDs *= ID;
  }


  std::cout << "Product of corner-tile IDs is " << product_of_corner_IDs
            << std::endl;


  return EXIT_SUCCESS;
}

