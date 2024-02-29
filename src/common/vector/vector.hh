#ifndef COMMON_VECTOR_VECTOR_HH
#define COMMON_VECTOR_VECTOR_HH
#include <vector>
using namespace std;

double calculateStandardDeviation(const vector<int>& numbers);
double calculateMeanLinesPerBank(const vector<int>& numbers);
double calculateMean(const std::vector<int>& numbers) ;
double calculateMean(const std::vector<double>& numbers) ;
double calculateGeoMean(const std::vector<double>& numbers);
double calculateEntropy(const vector<int>& numbers);

#endif // COMMON_VECTOR_VECTOR_HH