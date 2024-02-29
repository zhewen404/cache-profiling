#include "common/vector/vector.hh"
#include <math.h>

double calculateStandardDeviation(const vector<int>& numbers) {
    // Calculate the mean (average) of the numbers
    double sum = 0.0;
    for (int number : numbers) {
        sum += number;
    }
    double mean = sum / numbers.size();

    // Calculate the sum of squared differences from the mean
    double squaredDifferencesSum = 0.0;
    for (int number : numbers) {
        double difference = number - mean;
        squaredDifferencesSum += difference * difference;
    }

    // Calculate the variance
    double variance = squaredDifferencesSum / numbers.size();

    // Calculate the standard deviation (square root of the variance)
    double standardDeviation = sqrt(variance);

    return standardDeviation;
}

double calculateMean(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        // Handle the case when the vector is empty
        return 0.0; // You can choose to return any default value in this case
    }

    int sum = 0;
    int length = 0;
    for (int number : numbers) {
        sum += number;
        length += 1;
    }

    double mean = (double)sum / length;

    return mean;
}

double calculateMean(const std::vector<double>& numbers) {
    if (numbers.empty()) {
        // Handle the case when the vector is empty
        return 0.0; // You can choose to return any default value in this case
    }

    double sum = 0;
    int length = 0;
    for (double number : numbers) {
        sum += number;
        length += 1;
    }

    double mean = sum / length;

    return mean;
}


double calculateGeoMean(const std::vector<double>& numbers) {
    double product = 1.0;
    for (double number : numbers) {
        product *= number;
    }

    double geoMean = pow(product, 1.0 / numbers.size());

    return geoMean;
}

double calculateMeanLinesPerBank(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        // Handle the case when the vector is empty
        return 0.0; // You can choose to return any default value in this case
    }

    double sum = 0.0;
    int nonemptyset = 0;
    for (int number : numbers) {
        sum += number;
        if (number != 0) nonemptyset += 1;
    }

    double mean = sum / nonemptyset;

    return mean;
}

double calculateEntropy(const vector<int>& numbers) {
    double entropy = 0.0;
    int sum = 0;
    for (int number : numbers) {
        sum += number;
    }

    for (int number : numbers) {
        double probability = (double)number / sum;
        entropy -= probability * log2(probability);
    }
    return entropy;

}


