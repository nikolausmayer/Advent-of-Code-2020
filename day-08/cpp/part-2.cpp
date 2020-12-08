#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
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
    : argument(argument)
  { }

  virtual void execute(State&) = 0;
  
  unsigned visited{0};

  int argument{0};
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
    s.accumulator += argument;
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
    s.position += argument;
    ++visited;
  }
};

class Program
{
  public:
  enum class RET {
    OK, BAD, STOP
  };

  Program(const std::vector<std::shared_ptr<Instruction>>& i,
          State state)
    : m_state(state),
      m_instructions(i)
  { }

  bool run()
  {
    while (true) {
      auto ret{step()};
      switch (ret) {
        case RET::STOP: return true;
        case RET::BAD: return false;
      }
    }
  }

  RET step()
  {
    auto& next{m_instructions.at(m_state.position)};
    next->execute(m_state);

    if (m_state.position == m_instructions.size()) {
      std::cout << "Terminate with accumulator value: "
                << m_state.accumulator
                << std::endl;
      return RET::STOP;
    }

    if (m_state.position > m_instructions.size() or
        m_instructions.at(m_state.position)->visited > 0) {
      return RET::BAD;
    }

    return RET::OK;
  }

  State getState()
  {
    return m_state;
  }

  private:
  State m_state;
  std::vector<std::shared_ptr<Instruction>> m_instructions;
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

  std::vector<std::shared_ptr<Instruction>> instructions;
  while (true) {
    std::string opname;
    int oparg;
    infile >> opname >> oparg;
    if (infile.eof()) break;
    if      (opname == "acc")
      instructions.emplace_back(std::make_shared<ACC>(oparg));
    else if (opname == "jmp")
      instructions.emplace_back(std::make_shared<JMP>(oparg));
    else if (opname == "nop")
      instructions.emplace_back(std::make_shared<NOP>(oparg));
    else
      throw std::runtime_error(opname);
  }


  /// Run program and record states
  std::list<State> states;
  {
    Program p{instructions, State{}};
    while (p.step() != Program::RET::BAD)
      states.push_back(p.getState());
  }

  /// "Unwind" states to find the broken instruction
  while (states.size()) {
    auto last_state{states.back()};
    states.pop_back();

    auto failed_at{last_state.position};
    auto new_instrs{instructions};
    auto failed_instruction{new_instrs.at(failed_at)};
    auto iarg{failed_instruction->argument};
    if (std::dynamic_pointer_cast<NOP>(failed_instruction)) {
      new_instrs.at(failed_at) = std::make_shared<JMP>(iarg);
    } else if (std::dynamic_pointer_cast<JMP>(failed_instruction)) {
      new_instrs.at(failed_at) = std::make_shared<NOP>(iarg);
    } else {
      continue;
    }
    
    Program test{new_instrs, last_state};
    if (test.run()) break;
  }

  return EXIT_SUCCESS;
}

