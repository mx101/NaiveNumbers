#pragma once

#include <map>

constexpr size_t IMAGE_SIZE = 28;
constexpr size_t NUM_CLASSES = 10;  // 0-9 inclusive
constexpr double SHADED = 1.0;
constexpr double NOT_SHADED = 0.0;

/**
 * Represents a Naive Bayes classification model for determining the
 * likelihood that an individual pixel for an individual class is
 * white or black.
 */
struct Model {
  // The individual probabilities for each pixel for each class for
  // whether it's shaded or not.
  //
  // Examples:
  // probs_[2][10][7][0] is the computed probability that a pixel at
  // [2][10] for class 7 is not shaded.
  //
  // probs_[0][0][0][1] is the computed probability that a pixel at
  // [0][0] for class 0 is shaded.
  double probs_[IMAGE_SIZE][IMAGE_SIZE][NUM_CLASSES];
  std::map<int, double> num_class_counts_;
};
