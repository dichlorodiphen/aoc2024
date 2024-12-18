#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void error(const std::string& what) {
  std::cerr << what << '\n';
  std::exit(1);
}

std::vector<std::vector<int>> parse(const std::string name) {
  std::ifstream is{name};
  std::vector<std::vector<int>> reports;
  for (std::string l; std::getline(is, l);) {
    std::vector<int> report;
    std::stringstream ss{l};
    for (int level; ss >> level;) {
      report.push_back(level);
    }
    reports.push_back(report);
  }
  if (!is.eof()) {
    error("Error reading from input file.");
  }

  return reports;
}

bool isVarianceAcceptable(const std::vector<int>& report) {
  for (int i = 1; i < report.size(); ++i) {
    const auto diff = std::abs(report[i] - report[i - 1]);
    if (diff < 1 || diff > 3) {
      return false;
    }
  }
  return true;
}

bool isSafe(const std::vector<int>& report) {
  const auto isMonotonic = std::ranges::is_sorted(report) ||
                           std::ranges::is_sorted(report, std::greater<>());
  return isMonotonic && isVarianceAcceptable(report);
}

bool isSafeWithDampener(const std::vector<int>& report) {
  for (int i = 0; i < report.size(); ++i) {
    auto copy = report;
    copy.erase(copy.begin() + i);
    if (isSafe(copy)) {
      return true;
    }
  }
  return false;
}

int main() {
  const auto reports = parse("input");
  int safe = 0;
  for (const auto r : reports) {
    if (isSafeWithDampener(r)) {
      ++safe;
    }
  }

  std::cout << std::format("Number of safe reports: {}\n", safe);

  return 0;
}
