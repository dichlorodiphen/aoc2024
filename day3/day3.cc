#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

[[noreturn]] void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

struct Mul {
  int x;
  int y;
};

Mul parseMul(const std::string& s) {
  const auto after = s.substr(4);
  std::istringstream is{after};
  char delim = 0, right = 0;
  int x = 0, y = 0;
  is >> x >> delim >> y >> right;
  if (is && delim == ',' && right == ')') {
    return {x, y};
  }
  error("Failed to parse a mul");
}

std::vector<Mul> parse() {
  std::ifstream is{"input"};
  if (!is) {
    error("Failed to open.");
  }

  std::regex pattern{R"(don't\(\)|do\(\)|mul\(\d+,\d+\))"};
  std::vector<Mul> muls;
  bool enabled = true;
  for (std::string l; std::getline(is, l);) {
    std::sregex_iterator it{l.begin(), l.end(), pattern};
    std::sregex_iterator end;
    while (it != end) {
      auto token = it->str();
      std::cout << token << '\n';
      if (token.starts_with("mul")) {
        if (enabled) {
          muls.push_back(parseMul(token));
        }
      } else if (token == "don't()") {
        enabled = false;
      } else if (token == "do()") {
        enabled = true;
      } else {
        error("Unknown token: " + token);
      }
      ++it;
    }
  }
  return muls;
}

int main() {
  const auto muls = parse();
  for (const auto m : muls) {
    std::cout << m.x << ' ' << m.y << '\n';
  }

  const auto v =
      muls | std::views::transform([](const Mul m) { return m.x * m.y; });
  const auto sum = std::accumulate(v.begin(), v.end(), 0);

  std::cout << "The sum: " << sum << '\n';

  return 0;
}
