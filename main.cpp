#include "trainer.hpp"
#include "windows.h"
#include "stdio.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  int training_number = 3;
  int classifying_number = 4;

  if (argc == training_number) {
    std::cout << "Training..." << std::endl;
    Trainer trained;


    // Generate the vector and map count of integers each Image corresponds to
    std::vector<std::vector<bool>> ret_grid;
    std::vector<int> num_classes;
    std::map<int, double> num_counts;

    std::string label_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\digit_data\)";
    label_path += argv[2];
    std::ifstream file(label_path);

    trained.CreateNumCount(file, num_classes, num_counts);


    // Generate the vector of models for the images we are looking at
    std::string image_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\digit_data\)";
    image_path += argv[1];

    std::vector<Model> individual_models = trained.SplitToModels(image_path, num_classes);

    trained.final_model_ = Trainer::CombineModels(individual_models, num_classes, num_counts);
    trained.final_model_.num_class_counts_ = num_counts;


    // Write generated model to file
    std::ofstream out_file;
    out_file.open("model.txt");
    out_file << trained;

    // note that this is only used to tell user where the model is stored to
    TCHAR path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    std::string model_path = std::string(path);
    model_path += "\\model.txt";

    std::cout << "Saving Trained Model to: \n" << model_path << std::endl;

    out_file.close();

  } else if (argc == classifying_number) {
    Trainer trained;

    std::string model_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\cmake-build-debug\)";
    model_path += argv[1];
    std::ifstream model_file(model_path);

    model_file >> trained;

    std::string images_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\digit_data\)";
    images_path += argv[2];

    std::vector<int> classified_labels;
    std::vector<std::vector<bool>> all_images = FileReader::CreateGrid(images_path);
    std::vector<std::vector<bool>> single_image;

    for (int i = 0; i < all_images.size(); i++) {
      single_image.push_back(all_images[i]);

      if (single_image.size() % IMAGE_SIZE == 0 && i != 0) {
        int classification = trained.ClassifyImage(trained.final_model_, single_image);
        classified_labels.push_back(classification);
        single_image.clear();
      }
    }

    std::string correct_labels_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\digit_data\)";
    correct_labels_path += argv[3];

    std::vector<int> correct_labels = trained.FindLabels(correct_labels_path);

    double correct_count = 0;

    for (int i = 0; i < correct_labels.size(); i++) {
      if (classified_labels[i] == correct_labels[i]) {
        correct_count++;
      }
    }

    double percentage_correct = 100 * correct_count / correct_labels.size();

    std::cout << "The model was ";
    std::cout << percentage_correct << "% correct with the set of given test images." << std::endl;

  } else {

    std::cout << "Not enough arguments were provided" << std::endl;

  }

  return EXIT_SUCCESS;
}