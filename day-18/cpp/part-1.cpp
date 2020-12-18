#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>


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

  std::stack<long long> stack;

  auto num{0ll};
  while (true) {
    auto c{infile.get()};
    if (infile.eof()) break;
    switch (c) {
      case ' ': {
        if (stack.size() and stack.top() < -1 and num > 0) {
          auto op{stack.top()};  stack.pop();
          auto lhd{stack.top()}; stack.pop();
          if (op == -2) {
            num += lhd;
          } else {
            num *= lhd;
          }
        }
        break;
      }
      case '\n': {
        if (stack.size() and stack.top() < 0 and num > 0) {
          auto op{stack.top()};  stack.pop();
          auto lhd{stack.top()}; stack.pop();
          if (op == -2) {
            num += lhd;
          } else {
            num *= lhd;
          }
        }
        full_sum += num;
        num = 0;
        while (stack.size()) stack.pop();
        break;
      }
      case '0'...'9': {
        num = num*10 + (c - '0');
        break;
      }
      case '(': {
        stack.push(-1);
        num = 0;
        break;
      }
      case ')': {
        auto op{stack.top()};  stack.pop();
        auto lhd{stack.top()}; stack.pop();
        stack.pop();  /// '('
        if (op == -2) {
          num += lhd;
        } else {
          num *= lhd;
        }
        break;
      }
      case '+': {
        stack.push(num);
        stack.push(-2);
        num = 0;
        break;
      }
      case '*': {
        stack.push(num);
        stack.push(-3);
        num = 0;
        break;
      }
    }
  }

  std::cout << "Sum of results: " << full_sum << std::endl;

  return EXIT_SUCCESS;
}

