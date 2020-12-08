#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class State
{
  public:
  int accumulator{0};
  unsigned position{0};
};

class Instruction
{
  public:
  Instruction(int argument)
    : m_argument(argument)
  { }

  virtual void execute(State&) = 0;
  
  unsigned visited{0};

  protected:
  int m_argument{0};
};

class NOP : public Instruction
{
  using Instruction::Instruction;

  public:
  void execute(State& s) override
  {
    ++s.position;
    ++visited;
  }
};

class ACC : public Instruction
{
  using Instruction::Instruction;

  public:
  void execute(State& s) override
  {
    s.accumulator += m_argument;
    ++s.position;
    ++visited;
  }
};

class JMP : public Instruction
{
  using Instruction::Instruction;

  public:
  void execute(State& s) override
  {
    s.position += m_argument;
    ++visited;
  }
};

class Program
{
  public:
  Program(std::vector<std::unique_ptr<Instruction>> i)
    : m_instructions(std::move(i))
  { }

  void run()
  {
    while (true) {
      auto& next{m_instructions.at(m_state.position)};
      if (next->visited > 0) {
        std::cout << "Accumulator value before loop: "
                  << m_state.accumulator
                  << std::endl;
        return;
      }
      next->execute(m_state);
    }
  }

  private:
  State m_state;
  std::vector<std::unique_ptr<Instruction>> m_instructions;
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

  std::vector<std::unique_ptr<Instruction>> instructions;
  while (true) {
    std::string opname;
    int oparg;
    infile >> opname >> oparg;
    if (infile.eof()) break;
    if      (opname == "acc")
      instructions.emplace_back(std::make_unique<ACC>(oparg));
    else if (opname == "jmp")
      instructions.emplace_back(std::make_unique<JMP>(oparg));
    else if (opname == "nop")
      instructions.emplace_back(std::make_unique<NOP>(oparg));
    else
      throw std::runtime_error(opname);
  }
  
  Program program{std::move(instructions)};
  program.run();

  return EXIT_SUCCESS;
}

