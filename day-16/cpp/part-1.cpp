#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
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
  while (true) {
    std::string fieldname, dummy;
    int l1, h1, l2, h2;
    std::getline(infile, fieldname, ':');
    infile >> l1 >> h1 >> dummy >> l2 >> h2;
    fields.emplace_back(fieldname, l1, -h1, l2, -h2);
    infile.get();
    //std::cout << fields.back() << std::endl;
    if (infile.peek() == '\n')
      break;
  }

  /// Ignore our own ticket
  {
    std::string dummy;
    for (auto i{0}; i < 5; ++i)
      std::getline(infile, dummy);
  }

  auto ticket_scanning_error_rate{0};

  while (not infile.eof()) {
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
      if (not valid)
        ticket_scanning_error_rate += value;
    }
  }
  
  std::cout << "Ticket scanning error rate is "
            << ticket_scanning_error_rate
            << std::endl;

  return EXIT_SUCCESS;
}

