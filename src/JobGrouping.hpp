#ifndef JOB_GROUPING_HPP
#define JOB_GROUPING_HPP

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

class JobGrouping {
 public:
  JobGrouping(double threshold);
  std::vector<std::vector<int>> clusterPrograms(
      const std::vector<std::vector<std::string>>& programs);
  static double jaccardSimilarity(const std::vector<std::string>& a,
                                  const std::vector<std::string>& b);

 private:
  double similarityThreshold;
};

#endif  