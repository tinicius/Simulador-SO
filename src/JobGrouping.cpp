#include "JobGrouping.hpp"

JobGrouping::JobGrouping(double threshold) : similarityThreshold(threshold) {}

double JobGrouping::jaccardSimilarity(const std::vector<std::string>& a,
                                      const std::vector<std::string>& b) {
  std::unordered_set<std::string> setA(a.begin(), a.end());
  std::unordered_set<std::string> setB(b.begin(), b.end());

  std::unordered_set<std::string> intersection, unionSet;

  for (const auto& token : setA) {
    if (setB.count(token)) intersection.insert(token);
    unionSet.insert(token);
  }

  for (const auto& token : setB) {
    unionSet.insert(token);
  }

  return (double)intersection.size() / unionSet.size();
}

std::vector<std::vector<int>> JobGrouping::clusterPrograms(
    const std::vector<std::vector<std::string>>& programs) {
  std::vector<std::vector<int>> clusters;
  std::unordered_set<int> visited;

  for (size_t i = 0; i < programs.size(); i++) {
    if (visited.count(i)) continue;

    std::vector<int> cluster = {static_cast<int>(i)};
    visited.insert(i);

    for (size_t j = i + 1; j < programs.size(); j++) {
      if (!visited.count(j) &&
          jaccardSimilarity(programs[i], programs[j]) > similarityThreshold) {
        cluster.push_back(j);
        visited.insert(j);
      }
    }
    clusters.push_back(cluster);
  }
  return clusters;
}