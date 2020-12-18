#include <cstdlib>
#include <fstream>
#include <iostream>
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

  auto full_sum{0ll};

  std::vector<long long> stack;

  auto num{0ll};
  while (true) {
    auto c{infile.get()};
    if (infile.eof()) break;
    switch (c) {
      case ' ': {
        if (num > 0) {
          stack.push_back(num);
          num = 0;
        }
        break;
      }
      case '\n': {
        if (num > 0)
          stack.push_back(num);

        while (stack.size() > 1) {
          bool repeat{false};

          for (auto it{stack.begin()}; it != stack.end(); ++it)
            // ( A + B )
            if (stack.size() >= 5 and
                (*it == -2) and
                (*(it-2) == -1 and *(it+2) == -4)) {
              auto v{*(it-1) + *(it+1)};
              auto insertpoint{it-2};
              stack.erase(it-2, it+3);
              stack.insert(insertpoint, v);
              repeat = true;
              break;
            }

          if (repeat) continue;

          for (auto it{stack.begin()}; it != stack.end(); ++it)
            // ( A * B )
            if (stack.size() >= 5 and
                (*it == -3) and
                (*(it-2) == -1 and *(it+2) == -4)) {
              auto v{*(it-1) * *(it+1)};
              auto insertpoint{it-2};
              stack.erase(it-2, it+3);
              stack.insert(insertpoint, v);
              repeat = true;
              break;
            }

          if (repeat) continue;

          for (auto it{stack.begin()}; it != stack.end(); ++it)
            // A + B
            if (stack.size() >= 3 and (*it == -2) and
                *(it-1) >= 0 and *(it+1) >= 0) {
              auto v{*(it-1) + *(it+1)};
              auto insertpoint{it-1};
              stack.erase(it-1, it+2);
              stack.insert(insertpoint, v);
              repeat = true;
              break;
            }

          if (repeat) continue;

          for (auto it{stack.begin()}; it != stack.end(); ++it)
            // A * B
            if (stack.size() >= 3 and (*it == -3) and
                *(it-1) >= 0 and *(it+1) >= 0) {
              if (stack.size() >= 5 and *(it+2) == -2)
                continue;
              auto v{*(it-1) * *(it+1)};
              auto insertpoint{it-1};
              stack.erase(it-1, it+2);
              stack.insert(insertpoint, v);
              repeat = true;
              break;
            }
        }

        full_sum += stack.front();

        num = 0;
        stack.clear();
        break;
      }
      case '0'...'9': {
        num = num*10 + (c - '0');
        break;
      }
      case '(': {
        stack.push_back(-1);
        num = 0;
        break;
      }
      case ')': {
        if (num > 0)
          stack.push_back(num);
        stack.push_back(-4);
        num = 0;
        break;
      }
      case '+': {
        stack.push_back(-2);
        break;
      }
      case '*': {
        stack.push_back(-3);
        break;
      }
    }
  }

  std::cout << "Sum of results: " << full_sum << std::endl;

  return EXIT_SUCCESS;
}

