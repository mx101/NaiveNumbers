//
// Created by mxdra on 10/29/2019.
//

#include "trainer.hpp"
#include <cmath>

Trainer::Trainer() {
  // sets default probability value to 0.24
  // purely for testing
  double my_favorite_number = 0.24;

  for (int num_class = 0; num_class < NUM_CLASSES; num_class++) {
    for (unsigned row = 0; row < IMAGE_SIZE; row++) {
      for (unsigned column = 0; column < IMAGE_SIZE; column++) {
        this->final_model_.probs_[row][column][num_class] = my_favorite_number;
      }
    }
  }
}

Model Trainer::TrainModel(std::vector<std::vector<bool>> current_image, int num_class) {
  Model return_model = Model();

  for (unsigned row = 0; row < IMAGE_SIZE; row++) {
    for (unsigned column = 0; column < IMAGE_SIZE; column++) {
      if (current_image[row][column]) {
        return_model.probs_[row][column][num_class] = SHADED;
      } else {
        return_model.probs_[row][column][num_class] = NOT_SHADED;
      }
    }
  }

  return return_model;
}

Model Trainer::CombineModels(std::vector<Model> models, std::vector<int> num_classes, std::map<int, double> num_count) {
  Model return_model = Model();

  for (int i = 0; i < models.size(); i++) {
    for (unsigned row = 0; row < IMAGE_SIZE; row++) {
      for (unsigned column = 0; column < IMAGE_SIZE; column++) {
        return_model.probs_[row][column][num_classes[i]] +=
            (models[i].probs_[row][column][num_classes[i]] / num_count.find(num_classes[i])->second);
      }
    }
  }

  double laplace = 0.001;

  for (int num_class = 0; num_class < NUM_CLASSES; num_class++) {
    for (unsigned row = 0; row < IMAGE_SIZE; row++) {
      for (unsigned column = 0; column < IMAGE_SIZE; column++) {
        double value_pair = num_count.find(num_class)->second;
        double num_raw = return_model.probs_[row][column][num_class];

        double numerator = num_raw * value_pair;
        double laplace_denominator = num_count.find(num_class)->second + (2 * laplace);

        return_model.probs_[row][column][num_class] =
            (laplace + numerator) / laplace_denominator;
      }
    }
  }

  return return_model;
}

std::istream &operator>>(std::istream &in_stream, Trainer &model) {
  std::string curr_probability;

  int row = 0;
  int column = 0;
  int num_class = 0;

  while (in_stream >> curr_probability) {
    model.final_model_.probs_[row][column][num_class] = stod(curr_probability);
    column++;

    if (column == IMAGE_SIZE) {
      row++;
      column = 0;
    }

    if (row == IMAGE_SIZE) {
      num_class++;
      row = 0;
    }

    if (num_class == NUM_CLASSES) {
      break;
    }
  }

  std::string curr_key;
  std::string curr_val;

  while (in_stream >> curr_key) {
    in_stream >> curr_val;
    model.final_model_.num_class_counts_.insert(std::pair<int, double> (stod(curr_key), stod(curr_val)));
  }

  return in_stream;
}

std::ostream &operator<<(std::ostream &out_stream, const Trainer &model) {
  for (int num_class = 0; num_class < NUM_CLASSES; num_class++) {
    for (unsigned row = 0; row < IMAGE_SIZE; row++) {
      for (unsigned column = 0; column < IMAGE_SIZE; column++) {
        out_stream << std::to_string(model.final_model_.probs_[row][column][num_class]);
        out_stream << std::endl;
      }
    }
  }

  for (int num_class = 0; num_class < NUM_CLASSES; num_class++) {
    out_stream << std::to_string(model.final_model_.num_class_counts_.find(num_class)->first);
    out_stream << std::endl;
    out_stream << std::to_string(model.final_model_.num_class_counts_.find(num_class)->second);
    out_stream << std::endl;
  }

  return out_stream;
}

std::vector<Model> Trainer::SplitToModels(std::string image_path, std::vector<int> num_classes) {
  std::vector<std::vector<bool>> all_models_in_file = FileReader::CreateGrid(image_path);
  std::vector<Model> individual_models;
  int num_class_idx = 0;

  std::vector<std::vector<bool>> curr_image;
  Model to_models;

  for (unsigned long index = 0; index < all_models_in_file.size(); index++) {
    curr_image.push_back(all_models_in_file[index]);

    if (curr_image.size() % IMAGE_SIZE == 0 && index != 0) {
      to_models = Trainer::TrainModel(curr_image, num_classes[num_class_idx]);
      num_class_idx++;
      individual_models.push_back(to_models);
      curr_image.clear();
    }
  }

  return individual_models;
}

int Trainer::ClassifyImage(Model model, std::vector<std::vector<bool>> image_grid) {
  std::vector<double> posterior_vec;
  double total_in_model = 0;

  for (int i = 0; i < NUM_CLASSES; i++) {
    total_in_model += model.num_class_counts_.find(i)->second;
  }

  for (int num_class = 0; num_class < NUM_CLASSES; num_class++) {
    double p_class = model.num_class_counts_.find(num_class)->second / total_in_model;
    double posterior_probability = log(p_class);

    for (unsigned row = 0; row < IMAGE_SIZE; row++) {
      for (unsigned column = 0; column < IMAGE_SIZE; column++) {
        if (image_grid[row][column]) {
          posterior_probability += log(model.probs_[row][column][num_class]);
        } else {
          posterior_probability += log(1.0 - model.probs_[row][column][num_class]);
        }
      }
    }

    posterior_vec.push_back(exp(posterior_probability));
  }

  int highest_index = 0;
  double highest_prob = 0;

  for (int i = 0; i < posterior_vec.size(); i++) {
    if (posterior_vec[i] > highest_prob) {
      highest_index = i;
      highest_prob = posterior_vec[i];
    }
  }

  return highest_index;
}

std::vector<int> Trainer::FindLabels(std::string label_path) {
  std::vector<int> return_vector;
  
  std::ifstream labels_file(label_path);
  std::string current_label;

  while (labels_file >> current_label) {
    return_vector.push_back(stod(current_label));
  }
  
  return return_vector;
}

void Trainer::CreateNumCount(std::ifstream &file, std::vector<int> &num_classes, std::map<int, double> &num_counts) {
  std::string curr_line;

  // read each line of the file
  // create a bool vector for each line that gets appended to our grid
  while (std::getline(file, curr_line)) {
    int curr_int = stod(curr_line);
    num_classes.push_back(curr_int);

    auto found_itr = num_counts.find(curr_int);

    if (found_itr == num_counts.end()) {
      num_counts.insert(std::pair<int, double> (curr_int, 1));
    } else {
      double previous_count = found_itr->second;
      num_counts.erase(found_itr->first);
      num_counts.insert(std::pair<int, double> (curr_int, previous_count + 1));
    }
  }
}
