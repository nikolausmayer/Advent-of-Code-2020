#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class Range
{
public:
  constexpr Range(int low, int high)
    : m_low(low), m_high(high)
  { }

  constexpr bool contains(int i) const
  {
    return m_low <= i and i <= m_high;
  }

  friend std::ostream& operator<<(std::ostream&, const Range&);

private:
  int m_low, m_high;
};
std::ostream& operator<<(std::ostream& os, const Range& r)
{
  os << "[" << r.m_low << "-" << r.m_high << "]";
  return os;
}


class Field
{
public:
  Field(const std::string& name, int l1, int h1, int l2, int h2)
    : m_name(name),
      m_ranges{{{l1, h1}, {l2, h2}}}
  { }

  constexpr bool contains(int i) const
  {
    return m_ranges[0].contains(i) or
           m_ranges[1].contains(i);
  }
  
  constexpr const std::string& getName() const
  {
    return m_name;
  }

  friend std::ostream& operator<<(std::ostream&, const Field&);

private:
  std::string m_name;
  std::array<Range, 2> m_ranges;
};
std::ostream& operator<<(std::ostream& os, const Field& f)
{
  os << f.m_name << ": " 
     << f.m_ranges[0] << " "
     << f.m_ranges[1];
  return os;
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

  std::vector<Field> fields;
  std::set<std::string> open_fields;
  while (true) {
    std::string fieldname, dummy;
    int l1, h1, l2, h2;
    std::getline(infile, fieldname, ':');
    infile >> l1 >> h1 >> dummy >> l2 >> h2;
    fields.emplace_back(fieldname, l1, -h1, l2, -h2);
    open_fields.insert(fieldname);
    infile.get();
    if (infile.peek() == '\n') {
      std::getline(infile, dummy);
      std::getline(infile, dummy);
      break;
    }
  }

  std::vector<std::vector<int>> tickets;
  {
    std::vector<int> my_ticket;
    for (auto fidx{0u}; fidx < fields.size(); ++fidx) {
      int value;
      infile >> value;
      char c;
      if (fidx < fields.size()-1)
        infile >> c;
      my_ticket.push_back(value);
    }
    tickets.emplace_back(std::move(my_ticket));
  }
  {
    std::string dummy;
    std::getline(infile, dummy);
    std::getline(infile, dummy);
    std::getline(infile, dummy);
  }

  while (not infile.eof()) {
    std::vector<int> ticket;
    for (auto fidx{0u}; fidx < fields.size(); ++fidx) {
      auto valid{false};
      int value;
      infile >> value;
      if (infile.eof()) break;
      char c;
      if (fidx < fields.size()-1)
        infile >> c;
      for (const auto& field : fields)
        valid |= field.contains(value);
      if (valid)
        ticket.push_back(value);
    }
    if (ticket.size() == fields.size())
      tickets.emplace_back(std::move(ticket));
  }

  auto product{1ull};
  while (open_fields.size()) {
    for (auto col{0u}; col < fields.size(); ++col) {
      auto col_matches{0};
      std::string match_col;
      int match_fidx{0};
      for (auto fidx{0u}; fidx < fields.size(); ++fidx) {
        const auto& field{fields[fidx]};
        if (open_fields.find(field.getName()) == open_fields.end())
          continue;
        auto valid{true};
        for (const auto& row: tickets)
          if (not field.contains(row[col]))
            valid = false;
        if (valid) {
          ++col_matches;
          match_col = field.getName();
          match_fidx = col;
        }
      }
      if (col_matches == 1) {
        open_fields.erase(match_col);

        if (match_col.size() > std::string{"departure"}.size() and
            match_col.substr(0, std::string{"departure"}.size()) == "departure") {
          product *= tickets[0][match_fidx];
        }

        /// Restart cycle
        break;
      }
    }
  }

  std::cout << "Answer: " << product << std::endl;

  return EXIT_SUCCESS;
}

