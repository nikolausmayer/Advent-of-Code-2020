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
  virtual std::vector<std::string> matches(const std::string&) const = 0;
};


class SimpleRule : public Rule
{
public:
  void setText(std::string s)
  { 
    m_text = s;
  }

  std::vector<std::string> matches(const std::string& s) const override
  {
    if (s.starts_with(m_text)) {
      return {s.substr(m_text.size())};
    } 
    return {};
  }

private:
  std::string m_text;
};


class ConcatRule : public Rule
{
public:
  std::vector<std::string> matches(const std::string& s) const override
  {
    std::vector<std::string> results;
    std::vector<std::string> candidates{s};
    for (auto sub : m_subrules) {
      for (auto c : candidates) {
        auto tmp{rules[sub]->matches(c)};
        for (auto t : tmp)
          results.push_back(t);
      }
      if (results.size() == 0)
        return {};
      results.swap(candidates);
      results = {};
    }
    return candidates;
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
  std::vector<std::string> matches(const std::string& s) const override
  {
    std::vector<std::string> results;
    for (auto sub : m_subrules) {
      auto copy{s};
      auto tmp{rules[sub]->matches(copy)};
      for (auto t : tmp)
        results.push_back(t);
    }
    return results;
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

      if (name == 8 or name == 11)
        continue;
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


  /// New rule 8
  {
    auto r = std::make_shared<OptionRule>();
    {
      auto r1 = std::make_shared<ConcatRule>();
      r1->addSubrule(42);
      rules[anon_rules_counter] = r1;
      r->addSubrule(anon_rules_counter);
      --anon_rules_counter;
    }
    {
      auto r1 = std::make_shared<ConcatRule>();
      r1->addSubrule(42);
      r1->addSubrule(8);
      rules[anon_rules_counter] = r1;
      r->addSubrule(anon_rules_counter);
      --anon_rules_counter;
    }
    rules[8] = r;
  }
  /// New rule 11
  {
    auto r = std::make_shared<OptionRule>();
    {
      auto r1 = std::make_shared<ConcatRule>();
      r1->addSubrule(42);
      r1->addSubrule(31);
      rules[anon_rules_counter] = r1;
      r->addSubrule(anon_rules_counter);
      --anon_rules_counter;
    }
    {
      auto r1 = std::make_shared<ConcatRule>();
      r1->addSubrule(42);
      r1->addSubrule(11);
      r1->addSubrule(31);
      rules[anon_rules_counter] = r1;
      r->addSubrule(anon_rules_counter);
      --anon_rules_counter;
    }
    rules[11] = r;
  }


  auto matching_lines{0};

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) break;

    {
      auto name{0};
      auto rule{rules[name]};

      if (name < 0) continue;

      auto copy{line};
      for (auto s : rule->matches(copy)) {
        if (s == "") {
          ++matching_lines;
          break;
        }
      }
    }
  }

  std::cout << matching_lines << " messages match a rule." << std::endl;

  return EXIT_SUCCESS;
}

