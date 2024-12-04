#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
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

int search(const std::vector<char> s) {
  std::vector<char> xmas{'X', 'M', 'A', 'S'};
  int count = 0;
  auto pos = s.begin();
  while ((pos = std::search(pos, s.end(), xmas.begin(), xmas.end())) !=
         s.end()) {
    ++count;
    ++pos;
  }

  pos = s.begin();
  std::ranges::reverse(xmas);
  while ((pos = std::search(pos, s.end(), xmas.begin(), xmas.end())) !=
         s.end()) {
    ++count;
    ++pos;
  }

  return count;
}

int main() {
  const auto grid = parse();
  const auto num_rows = grid.size();
  const auto num_cols = grid[0].size();
  int count = 0;

  // Rows
  for (const auto row : grid) {
    count += search(row);
  }

  std::cout << std::format("Count after rows: {}\n", count);

  // Cols
  for (int c = 0; c < num_cols; ++c) {
    std::vector<char> col;
    for (int r = 0; r < num_rows; ++r) {
      col.push_back(grid[r][c]);
    }
    count += search(col);
  }

  std::cout << std::format("Count after cols: {}\n", count);

  // Main diags
  for (int r = 0; r < num_rows; ++r) {
    std::vector<char> diag;
    for (int i = r, j = 0; i < num_rows && j < num_cols; ++i, ++j) {
      diag.push_back(grid[i][j]);
    }
    count += search(diag);
  }
  for (int c = 1; c < num_cols; ++c) {
    std::vector<char> diag;
    for (int i = 0, j = c; i < num_rows && j < num_cols; ++i, ++j) {
      diag.push_back(grid[i][j]);
    }
    count += search(diag);
  }

  // Anti diags
  for (int r = 0; r < num_rows; ++r) {
    std::vector<char> diag;
    for (int i = r, j = 0; i < num_rows && j < num_cols; --i, ++j) {
      diag.push_back(grid[i][j]);
    }
    count += search(diag);
  }
  for (int c = 1; c < num_cols; ++c) {
    std::vector<char> diag;
    for (int i = num_rows - 1, j = c; i < num_rows && j < num_cols; --i, ++j) {
      diag.push_back(grid[i][j]);
    }
    count += search(diag);
  }

  std::cout << std::format("Number of matches: {}\n", count);

  return 0;
}
