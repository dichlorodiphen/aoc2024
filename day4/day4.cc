#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <span>
#include <string>
#include <utility>
#include <vector>

[[noreturn]] void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

std::vector<std::vector<char>> parse() {
  std::ifstream is{"input"};
  if (!is) {
    error("Failed to open input file for reading.");
  }

  std::vector<std::vector<char>> grid;
  for (std::string l; std::getline(is, l);) {
    std::vector<char> row;
    for (const auto c : l) {
      row.push_back(c);
    }
    grid.push_back(row);
  }
  if (is.bad()) {
    error("Failed to read from input stream.");
  }

  // All rows have same length?
  int len = grid[0].size();
  if (!std::ranges::all_of(grid, [len](auto v) { return v.size() == len; })) {
    error("Expected all rows to be same size");
  }

  return grid;
}

const std::vector<char> mas{'M', 'A', 'S'};
const std::vector<char> sam{mas.rbegin(), mas.rend()};

bool is_window_valid(std::pair<int, int> top_left,
                     std::vector<std::vector<char>> grid) {
  const auto [x, y] = top_left;
  std::vector<char> main{grid[x][y], grid[x + 1][y + 1], grid[x + 2][y + 2]};
  std::vector<char> anti{grid[x + 2][y], grid[x + 1][y + 1], grid[x][y + 2]};

  if ((main == mas || main == sam) && (anti == mas || anti == sam)) {
    return true;
  }

  return false;
}

int main() {
  const auto grid = parse();
  const auto num_rows = grid.size();
  const auto num_cols = grid[0].size();
  const auto win_size = 3;
  int count = 0;

  for (int i = 0; i < num_rows - win_size + 1; ++i) {
    for (int j = 0; j < num_cols - win_size + 1; ++j) {
      if (is_window_valid({i, j}, grid)) {
        ++count;
      }
    }
  }

  std::cout << std::format("Number of matches: {}\n", count);

  return 0;
}
