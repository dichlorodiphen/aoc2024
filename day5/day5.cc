#include <algorithm>
#include <cinttypes>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

[[noreturn]] void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

std::pair<std::vector<std::pair<int, int>>, std::vector<std::vector<int>>>
parse() {
  std::vector<std::pair<int, int>> pairs;
  std::ifstream is{"input"};

  std::string line;
  while (std::getline(is, line) && !line.empty()) {
    std::istringstream ss{line};
    int x = 0, y = 0;
    char delim = 0;
    ss >> x >> delim >> y;
    if (delim != '|') {
      std::cout << delim << '\n';
      error("Parse error, unknown delim.");
    }
    pairs.push_back({x, y});
  }

  std::vector<std::vector<int>> updates;
  while (std::getline(is, line)) {
    std::vector<int> update;
    std::istringstream ss{line};
    std::string x;
    while (std::getline(ss, x, ',')) {
      update.push_back(std::stoi(x));
    }
    updates.push_back(update);
  }

  return {pairs, updates};
}

using Graph = std::unordered_map<int, std::unordered_set<int>>;

std::pair<bool, int> isValid(std::vector<int>& update, Graph& deps) {
  std::unordered_set<int> cannot_contain;
  for (int i = update.size() - 1; i >= 0; --i) {
    if (cannot_contain.contains(update[i])) {
      return {false, i};
    }
    cannot_contain.insert_range(deps[update[i]]);
  }

  return {true, 0};
}

std::vector<int> correct(std::vector<int>& update, Graph& deps) {
  auto [is_valid, i] = isValid(update, deps);
  if (!is_valid) {
    std::swap(update[i], update[i + 1]);
    return correct(update, deps);
  }
  return update;
}

int main() {
  // Build graph
  auto [pairs, updates] = parse();
  std::unordered_map<int, std::unordered_set<int>> deps;
  for (auto [x, y] : pairs) {
    deps[x].insert(y);
  }

  int sum = 0;
  auto incorrect = updates | std::views::filter([&deps](auto u) {
                     return !isValid(u, deps).first;
                   });
  for (auto u : incorrect) {
    auto corrected = correct(u, deps);
    sum += corrected[corrected.size() / 2];
  }

  std::cout << "Sum: " << sum << '\n';

  return 0;
}
