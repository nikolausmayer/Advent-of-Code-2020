Another simple "game of life" puzzle, this time with one additional twist: the grid is (theoretically) infinite, so because I use a `std::map` to store the tile states, simply looping over the existing map keys would miss the rule for most "white" tiles (so now I seed all white neighbors of black tiles into the map before each iteration).