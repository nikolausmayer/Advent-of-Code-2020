#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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
    ///      0 ->
    ///      2 <-
    /// ^ |       ^ |
    /// | v       | v
    /// 5 4       7 6
    ///      1 ->
    ///      3 <-

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

  void cropEdges()
  {
    m_data.erase(m_data.begin());
    m_data.erase(m_data.begin()+m_data.size()-1);
    for (auto i{0u}; i < m_data.size(); ++i) 
      m_data[i] = m_data[i].substr(1, m_data[i].size()-2);
  }

  void flipX()
  {
    for (auto i{0u}; i < m_data.size(); ++i) 
      std::reverse(m_data[i].begin(), m_data[i].end());
  }

  void flipY()
  {
    std::reverse(m_data.begin(), m_data.end());
  }

  void rotR()
  {
    decltype(m_data) rotated;
    for (auto x{0u}; x < m_data[0].size(); ++x) {
      std::ostringstream oss;
      for (auto y{0u}; y < m_data.size(); ++y) {
        oss << m_data[m_data.size()-1-y][x];
      }
      rotated.push_back(oss.str());
    }

    m_data = rotated;
  }

  std::string getDataLine(unsigned int idx)
  {
    return m_data[idx];
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


  std::vector<int> corner_tiles;
  std::vector<int> edge_tiles;
  std::vector<int> inner_tiles;

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

    switch (matching_edges) {
      case 4: { corner_tiles.push_back(ID); break; }
      case 6: { edge_tiles.push_back(ID);   break; }
      case 8: { inner_tiles.push_back(ID);  break; }
    }
  }

  //auto isCornerTile = [&tiles](int ID) {
  //  int partnered_edges_count{0};
  //  for (auto edge : tiles[ID].getPossibleEdges()) {
  //    for (const auto& [ID2, tile2] : tiles) {
  //      if (ID2 == ID) continue;
  //      for (auto edge2 : tile2.getPossibleEdges()) {
  //        if (edge == edge2) {
  //          ++partnered_edges_count;
  //          break;
  //        }
  //      }
  //    }
  //  }
  //  return partnered_edges_count == 4;
  //};

  auto elementIndex = [](const std::vector<unsigned int>& vec, unsigned int el) -> unsigned {
    return std::distance(vec.begin(), std::find(vec.begin(), vec.end(), el));
  };

  /// Arrange compound image tiles
  std::vector<std::vector<int>> image;
  /// Assign some corner tile to upper-left
  {
    int upperleft{corner_tiles.back()};
    corner_tiles.pop_back();
    image.push_back({});
    image.back().push_back(upperleft);

    while (corner_tiles.size()) {
      edge_tiles.push_back(corner_tiles.back());
      corner_tiles.pop_back();
    }
    while (inner_tiles.size()) {
      edge_tiles.push_back(inner_tiles.back());
      inner_tiles.pop_back();
    }

    std::vector<unsigned int> partnered_edges;
    for (auto edge : tiles[upperleft].getPossibleEdges()) {
      for (auto eID : edge_tiles) {
        for (auto edge2 : tiles[eID].getPossibleEdges()) {
          if (edge == edge2) {
            partnered_edges.push_back(edge);
            break;
          }
        }
      }
    }

    ///      0 ->
    ///      2 <-
    /// ^ |       ^ |
    /// | v       | v
    /// 5 4       7 6
    ///      1 ->
    ///      3 <-

    std::vector<unsigned int> partnered_edge_indices;
    for (auto edge : partnered_edges) 
      partnered_edge_indices.push_back(elementIndex(tiles[upperleft].getPossibleEdges(), edge));
    if (std::find(partnered_edge_indices.begin(), partnered_edge_indices.end(), 6) != partnered_edge_indices.end()) {
      /// RL or RU
      if (std::find(partnered_edge_indices.begin(), partnered_edge_indices.end(), 1) != partnered_edge_indices.end()) {
        /// RL: Correct orientation, nothing to do
      } else {
        /// RU: Rotate right 1x
        tiles[upperleft].rotR();
      }
    } else {
      /// LL or LU
      if (std::find(partnered_edge_indices.begin(), partnered_edge_indices.end(), 1) != partnered_edge_indices.end()) {
        /// LL: Rotate right 3x
        tiles[upperleft].rotR();
        tiles[upperleft].rotR();
        tiles[upperleft].rotR();
      } else {
        /// LU: Rotate right 2x
        tiles[upperleft].rotR();
        tiles[upperleft].rotR();
      }
    }
  }

  int current_reference{image[0][0]};
  while (edge_tiles.size()) {
    auto ref_edge{tiles[current_reference].getPossibleEdges()[6]};
    bool edge_tile_found{false};
    for (auto eID : edge_tiles) {
      unsigned in_eID{elementIndex(tiles[eID].getPossibleEdges(), ref_edge)};
      switch (in_eID) {
        case 0: {
          tiles[eID].rotR();
          tiles[eID].flipX();
          break;
        }
        case 1: {
          tiles[eID].rotR();
          break;
        }
        case 2: {
          tiles[eID].rotR();
          tiles[eID].rotR();
          tiles[eID].rotR();
          break;
        }
        case 3: {
          tiles[eID].rotR();
          tiles[eID].flipY();
          break;
        }
        case 4: {
          /// Base case
          break;
        }
        case 5: {
          tiles[eID].flipY();
          break;
        }
        case 6: {
          tiles[eID].flipX();
          break;
        }
        case 7: {
          tiles[eID].rotR();
          tiles[eID].rotR();
          break;
        }
        default: continue;
      }

      edge_tile_found = true;
      image.back().push_back(eID);
      edge_tiles.erase(std::find(edge_tiles.begin(), edge_tiles.end(), eID));
      current_reference = eID;
      break;
    }
    if (not edge_tile_found) {
      ref_edge = tiles[image.back().front()].getPossibleEdges()[1];
      for (auto eID : edge_tiles) {
        unsigned in_eID{elementIndex(tiles[eID].getPossibleEdges(), ref_edge)};
        switch (in_eID) {
          case 0: {
            /// Base case
            break;
          }
          case 1: {
            tiles[eID].flipY();
            break;
          }
          case 2: {
            tiles[eID].flipX();
            break;
          }
          case 3: {
            tiles[eID].rotR();
            tiles[eID].rotR();
            break;
          }
          case 4: {
            tiles[eID].rotR();
            tiles[eID].flipX();
            break;
          }
          case 5: {
            tiles[eID].rotR();
            break;
          }
          case 6: {
            tiles[eID].rotR();
            tiles[eID].rotR();
            tiles[eID].rotR();
            break;
          }
          case 7: {
            tiles[eID].rotR();
            tiles[eID].flipY();
            break;
          }
          default: continue;
        }

        image.push_back({});
        image.back().push_back(eID);
        edge_tiles.erase(std::find(edge_tiles.begin(), edge_tiles.end(), eID));
        current_reference = eID;
        break;
      }
    }
  }
  

  /// Crop edges
  for (auto& [_, v] : tiles) v.cropEdges();


  /// Assemble big compound image
  std::vector<std::string> dataimage;
  while (image.size()) {
    std::ostringstream oss;
    for (auto lineidx{0u}; lineidx < 8; ++lineidx) {
      for (auto tileID : image.front()) {
        oss << tiles[tileID].getDataLine(lineidx);
      }
      dataimage.push_back(oss.str());
      oss.str("");
    }
    image.erase(image.begin());
  }


  /// Look for monster
  auto filterMonster = [](std::vector<std::string>& dataimage,
                          const std::vector<std::string>& monster) {
    auto found{0};
    const auto MH{monster.size()};
    const auto MW{monster[0].size()};
    const auto H{dataimage.size() - MH};
    const auto W{dataimage[0].size() - MW};

    for (auto y{0u}; y < H; ++y) {
      for (auto x{0u}; x < W; ++x) {

        auto monsterhere{true};
        for (auto dy{0u}; dy < MH and monsterhere; ++dy) {
          for (auto dx{0u}; dx < MW and monsterhere; ++dx) {
            if (monster[dy][dx] == '#' and 
                dataimage[y+dy][x+dx] != '#')
              monsterhere = false;
          }
        }

        if (monsterhere) {
          ++found;
          for (auto dy{0u}; dy < MH; ++dy) {
            for (auto dx{0u}; dx < MW; ++dx) {
              if (monster[dy][dx] == '#' and 
                  dataimage[y+dy][x+dx] == '#')
                dataimage[y+dy][x+dx] = 'O';
            }
          }
        }

      }
    }

    return found;
  };


  /// Monster
  std::vector<std::string> monster{
    "                  # ",
    "#    ##    ##    ###",
    " #  #  #  #  #  #   "
  };

  auto rotate = [](auto in) {
    decltype(in) rotated;
    for (auto x{0u}; x < in[0].size(); ++x) {
      std::ostringstream oss;
      for (auto y{0u}; y < in.size(); ++y) {
        oss << in[in.size()-1-y][x];
      }
      rotated.push_back(oss.str());
    }
    return rotated;
  };

  std::vector<std::vector<std::string>> monster_farm;
  monster_farm.push_back(monster);
  monster_farm.push_back(rotate(monster_farm.back()));
  monster_farm.push_back(rotate(monster_farm.back()));
  monster_farm.push_back(rotate(monster_farm.back()));
  for (auto i{0u}; i < monster.size(); ++i)
    std::reverse(monster[i].begin(), monster[i].end());
  monster_farm.push_back(monster);
  monster_farm.push_back(rotate(monster_farm.back()));
  monster_farm.push_back(rotate(monster_farm.back()));
  monster_farm.push_back(rotate(monster_farm.back()));

  for (auto monster : monster_farm) {
    auto copy(dataimage);
    auto monsters{filterMonster(copy, monster)};
    if (monsters > 0) {
      auto result{0};
      for (auto line : copy)
        for (auto c : line)
          result += (c == '#');
      std::cout << "Water roughness: " << result << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}

