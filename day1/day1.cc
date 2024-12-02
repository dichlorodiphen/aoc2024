#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

std::unordered_map<int, int> computeFrequencies(const std::vector<int>& ids) {
  std::unordered_map<int, int> freqs;
  for (const auto id : ids) {
    ++freqs[id];
  }

  return freqs;
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

  auto freqs = computeFrequencies(right);
  int similarity;
  for (const auto id : left) {
    similarity += id * freqs[id];
  }

  std::cout << std::format("Similarity score: {}\n", similarity);

  return 0;
}
