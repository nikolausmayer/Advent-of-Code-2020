#include <cstdlib>
#include <fstream>
#include <iostream>


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

  long long card_pk, door_pk;
  infile >> card_pk >> door_pk;

  auto step = [](auto value, auto subject_number) {
    return (value * subject_number) % 20201227ll;
  };

  auto subject_number{7ll};
  auto value{1ll};

  auto card_loopsize{0};
  while (value != card_pk) {
    value = step(value, subject_number);
    ++card_loopsize;
  }

  value = 1ll;
  auto door_loopsize{0};
  while (value != door_pk) {
    value = step(value, subject_number);
    ++door_loopsize;
  }


  value = 1ll;
  for (auto i{0}; i < door_loopsize; ++i)
    value = step(value, card_pk);

  std::cout << "Encryption key: " << value << std::endl;


  return EXIT_SUCCESS;
}

