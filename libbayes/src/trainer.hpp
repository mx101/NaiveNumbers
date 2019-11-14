//
// Created by mxdra on 10/29/2019.
//

#ifndef NAIVEBAYES_LIBBAYES_SRC_TRAINER_HPP_
#define NAIVEBAYES_LIBBAYES_SRC_TRAINER_HPP_

#include "model.hpp"
#include "file_reader.hpp"
#include <map>

class Trainer {
 public:
  /// Base constructor for testing purposes
  Trainer();

  /// Method to train the model
  /// Takes in a single image and generates a Model based off the one image
  static Model TrainModel(std::vector<std::vector<bool>>, int);

  /// Takes in a vector of "equally weighted" Models and combines their probabilities
  /// Equal weight in this case means the number of Images used to create the model
  /// were the same across every model in the vector
  static Model CombineModels(std::vector<Model>, std::vector<int>, std::map<int, double>);

  /// Takes in a file path and vector of expected int labels
  /// Generates a set of models that can be used in CombineModels()
  std::vector<Model> SplitToModels(std::string, std::vector<int>);

  /// Takes a model and 2D vector representing an image
  /// returns the int classification
  static int ClassifyImage(Model, std::vector<std::vector<bool>>);

  /// Takes in a file path and references to int vector and <int, double> map
  /// Takes the labels from the file and pushes them onto a vector of ints
  /// Counts the number of each int and puts this into the map
  void CreateNumCount(std::ifstream &file,
                         std::vector<int> &num_classes,
                         std::map<int, double> &num_counts);

  /// Takes a file path and returns a vector of the int labels found
  /// Used primarily for reading the labels when comparing a classified set with the actual
  std::vector<int> FindLabels(std::string label_path);

  friend std::istream& operator>>(std::istream &, Trainer &);
  friend std::ostream& operator<<(std::ostream &, const Trainer &);

  Model final_model_;
};

#endif //NAIVEBAYES_LIBBAYES_SRC_TRAINER_HPP_
