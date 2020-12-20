#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>


class Rule;
std::map<int, std::shared_ptr<Rule>> rules;
int anon_rules_counter{-1};


class Rule
{
public:
  virtual bool matches(std::string&) const = 0;
};


class SimpleRule : public Rule
{
public:
  void setText(std::string s)
  { 
    m_text = s;
  }

  bool matches(std::string& s) const override
  {
    if (s.starts_with(m_text)) {
      s = s.substr(m_text.size());
      return true;
    } 
    return false;
  }

private:
  std::string m_text;
};


class ConcatRule : public Rule
{
public:
  bool matches(std::string& s) const override
  {
    for (auto sub : m_subrules) {
      if (not rules[sub]->matches(s)) {
        return false;
      }
    }
    return true;
  }

  void addSubrule(int r)
  {
    m_subrules.push_back(r);
  }

private:
  std::vector<int> m_subrules;
};


class OptionRule : public Rule
{
public:
  bool matches(std::string& s) const override
  {
    for (auto sub : m_subrules) {
      auto copy{s};
      if (rules[sub]->matches(copy)) {
        s = copy;
        return true;
      }
    }
    return false;
  }

  void addSubrule(int r)
  {
    m_subrules.push_back(r);
  }

private:
  std::vector<int> m_subrules;
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



  std::map<int, int> copyrules;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (line == "") break;

    std::istringstream iss{line};
    int name;
    {
      std::string s;
      iss >> s;
      name = std::stoi(s.substr(0, s.size()-1));
    }

    std::vector<std::string> words;
    while (true) {
      std::string word;
      iss >> word;
      if (word == "") break;
      words.push_back(word);
    }
    
    if (words.size() == 1) {
      auto word{words[0]};
      if (word[0] == '"') {
        /// Simple rule
        auto rule = std::make_shared<SimpleRule>();
        rule->setText(word.substr(1, 1));
        rules[name] = rule;
      } else {
        /// Copy rule
        copyrules[name] = std::stoi(word);
      }
    } else {
      if (std::find(words.begin(), words.end(), "|") != words.end()) {
        /// Option rule
        auto rule = std::make_shared<OptionRule>();
        auto sep{0u};
        for (auto idx{0u}; idx < words.size(); ++idx)
          if (words[idx] == "|")
            sep = idx;

        auto srule1 = std::make_shared<ConcatRule>();
        for (auto idx{0u}; idx < sep; ++idx) {
          srule1->addSubrule(std::stoi(words[idx]));
        }
        rules[anon_rules_counter] = srule1;
        rule->addSubrule(anon_rules_counter);
        --anon_rules_counter;

        auto srule2 = std::make_shared<ConcatRule>();
        for (auto idx{sep+1}; idx < words.size(); ++idx) {
          srule2->addSubrule(std::stoi(words[idx]));
        }
        rules[anon_rules_counter] = srule2;
        rule->addSubrule(anon_rules_counter);
        --anon_rules_counter;

        rules[name] = rule;
      } else {
        /// Concat rule
        auto rule = std::make_shared<ConcatRule>();
        for (auto word : words) {
          rule->addSubrule(std::stoi(word));
        }
        rules[name] = rule;
      }
    }
  }

  for (auto [k, v] : copyrules)
    rules[k] = rules[v];

  auto matching_lines{0};

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) break;

    for (const auto& [name, rule] : rules) {
      if (name < 0) continue;

      auto copy{line};
      if (rule->matches(copy)) {
        if (copy == "") {
          ++matching_lines;
        }
      }
    }
  }

  std::cout << matching_lines << " messages match a rule." << std::endl;

  return EXIT_SUCCESS;
}

