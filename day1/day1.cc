#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

int main() {
  // Parse input
  std::ifstream is{"input"};
  std::vector<int> left, right;
  for (int l, r; is >> l >> r;) {
    left.push_back(l);
    right.push_back(r);
  }
  if (!is.eof()) {
    error("Failed to read from input file.");
  }
  std::ranges::sort(left);
  std::ranges::sort(right);

  // Calculate distances
  if (left.size() != right.size()) {
    error("Expected the two lists to be the same size.");
  }

  int total = 0;
  for (int i = 0; i < left.size(); ++i) {
    total += std::abs(right[i] - left[i]);
  }

  std::cout << std::format("Total distance: {}\n", total);

  return 0;
}
