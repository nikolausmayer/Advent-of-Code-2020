#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

typedef std::vector<std::string> VEC;


struct Food
{
  VEC ingredients;
  VEC allergens;
};
//std::ostream& operator<<(std::ostream& os, const Food& f) 
//{
//  for (auto i : f.ingredients) os << i << " ";
//  os << "| ";
//  for (auto a : f.allergens)   os << a << " ";
//  return os;
//}


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

  std::vector<Food> food_list{1};
  VEC current_parse;

  while (true) {
    std::string word;
    infile >> word;
    if (infile.eof()) break;
    if (word.front() == '(') {
      std::sort(current_parse.begin(), current_parse.end());
      food_list.back().ingredients = current_parse;
      current_parse.clear();
      continue;
    } else if (word.back() == ',') {
      word = word.substr(0, word.size()-1);
      current_parse.push_back(word);
    } else if (word.back() == ')') {
      word = word.substr(0, word.size()-1);
      current_parse.push_back(word);
      std::sort(current_parse.begin(), current_parse.end());
      food_list.back().allergens = current_parse;
      current_parse.clear();
      food_list.push_back({});
    } else {
      current_parse.push_back(word);
    }
  }
  food_list.pop_back();
  //for (const auto& f : food_list) {
  //  for (const auto& i: f.ingredients)
  //    std::cout << i << " ";
  //  std::cout << "( ";
  //  for (const auto& a: f.allergens)
  //    std::cout << a << " ";
  //  std::cout << ")" << std::endl;
  //}
  //std::cout << std::endl;

  auto contains = [](const auto& v, const auto& el) {
    return std::find(std::begin(v), std::end(v), el) != std::end(v);
  };

  VEC ingredients_list;
  VEC allergens_list;
  for (const auto& f : food_list) {
    for (const auto& i : f.ingredients)
      if (not contains(ingredients_list, i))
        ingredients_list.push_back(i);
    for (const auto& a : f.allergens)
      if (not contains(allergens_list, a))
        allergens_list.push_back(a);
  }
  std::sort(std::begin(ingredients_list), std::end(ingredients_list));
  std::sort(std::begin(allergens_list), std::end(allergens_list));

  auto isect = [](const auto& v1, const auto& v2) {
    typename std::remove_const<typename std::remove_reference<decltype(v1)>::type>::type result;
    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(result));
    return result;
  };

  auto diff = [](const auto& v1, const auto& v2) {
    typename std::remove_const<typename std::remove_reference<decltype(v1)>::type>::type result;
    std::set_difference(v1.begin(), v1.end(),
                        v2.begin(), v2.end(),
                        std::back_inserter(result));
    return result;
  };

  VEC ingredients_without_allergens{ingredients_list};
  for (const auto& allergen : allergens_list) {
    VEC ingredients_maybe_with_allergen{ingredients_list};
    for (const auto& food : food_list)
      if (contains(food.allergens, allergen))
        ingredients_maybe_with_allergen = isect(ingredients_maybe_with_allergen,
                                                food.ingredients);
    //std::cout << allergen << " may be in: ";
    //for (auto i : ingredients_maybe_with_allergen)
    //  std::cout << i << " ";
    //std::cout << std::endl;
    ingredients_without_allergens = diff(ingredients_without_allergens,
                                         ingredients_maybe_with_allergen);
  }
  //std::cout << std::endl;
  //std::cout << "Ingredients without allergens: ";
  //for (auto i : ingredients_without_allergens)
  //  std::cout << i << " ";
  //std::cout << std::endl;

  auto count{0};
  for (const auto& f : food_list)
    for (const auto& i : ingredients_without_allergens)
      count += contains(f.ingredients, i);

  std::cout << "Ingredients without allergens appear " << count << " times."
            << std::endl;

  return EXIT_SUCCESS;
}

