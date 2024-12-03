#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

struct Mul {
  int x;
  int y;
};

std::vector<Mul> parseLine(const std::string& s) {
  std::vector<Mul> muls;
  int pos = 0;
  while ((pos = s.find("mul(", pos)) != s.npos) {
    // Holy shit this is terrible
    const auto start = pos + 4;
    const auto after = s.substr(start);
    std::istringstream ss{after};
    int x, y;
    char delim = 0, right = 0;
    ss >> x >> delim >> y >> right;
    if (delim == ',' && right == ')' && ss) {
      muls.push_back({x, y});
    }
    ++pos;
  }

  return muls;
}

std::vector<Mul> parse() {
  std::ifstream is{"input"};
  if (!is) {
    error("Failed to open.");
  }

  std::vector<Mul> muls;
  for (std::string l; std::getline(is, l);) {
    for (const auto m : parseLine(l)) {
      muls.push_back(m);
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
