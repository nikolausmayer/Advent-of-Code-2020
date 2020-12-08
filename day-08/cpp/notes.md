The hardest day so far. Part 1 was easy enough, though I suspect that how one solves it affects the difficulty of part 2 a lot.
Part 2 was hard, but cool. The difficulty for me was about equal parts **having to think** and **knowing the solution but having to find the C++ syntax that allows it**. For example, I do not find it intuitive that to compare the types of two `std::shared_ptr`s, you invoke `std::dynamic_pointer_cast<A>(std::shared_ptr<B>)` and not `std::dynamic_pointer_cast<std::shared_ptr<A>>(std::shared_ptr<B>)`.