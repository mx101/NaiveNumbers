//
// Created by mxdra on 10/29/2019.
//

#include "catch.hpp"
#include "trainer.hpp"

SCENARIO("Train and combine two Models") {
  unsigned line_length = 28;

  std::vector<Model> models;
  std::vector<int> classes;
  int class_index = 0;

  std::vector<std::vector<bool>> empty_grid;
  Model empty_model;

  std::vector<std::vector<bool>> has_values_grid;
  Model first_line_model;

  //do setup of empty_model
  empty_grid = FileReader::CreateGrid("bad file path");
  empty_model = Trainer::TrainModel(empty_grid, class_index);

  models.push_back(empty_model);
  classes.push_back(class_index);


  GIVEN("A single empty 28x28 vector") {
    std::vector<std::vector<bool>> expected;

    for (unsigned i = 0; i < line_length; i++) {
      std::vector<bool> false_filled_vector(line_length, false);
      expected.push_back(false_filled_vector);
    }

    REQUIRE(empty_grid == expected);

    // note that index = 0 is just arbitrary,
    // I just needed one num_class to be able to access for testing

    THEN("Require a Model with all 0's") {
      for (unsigned row = 0; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          REQUIRE(empty_model.probs_[row][column][class_index] == 0.0);
        }
      }
    }
  }


  // do setup of first_line_model
  std::string file_path_single = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\single_line_file)";
  has_values_grid = FileReader::CreateGrid(file_path_single);

  first_line_model = Trainer::TrainModel(has_values_grid, class_index);

  models.push_back(first_line_model);
  classes.push_back(class_index);


  GIVEN("A file with one line with true positions") {
    std::vector<std::vector<bool>> expected;

    // setup distinct first line of file
    std::vector<bool> first_line;
    first_line.push_back(true);
    first_line.push_back(true);
    first_line.push_back(true);
    first_line.push_back(false);
    first_line.push_back(true);
    first_line.push_back(true);
    while (first_line.size() < line_length) {
      first_line.push_back(false);
    }
    expected.push_back(first_line);

    while(expected.size() < line_length) {
      std::vector<bool> false_filled_vector(line_length, false);
      expected.push_back(false_filled_vector);
    }

    REQUIRE(has_values_grid == expected);

    // note that index = 0 is just arbitrary,
    // I just needed one num_class to be able to access for testing

    THEN("Require a Model with correct 1 positions") {
      REQUIRE(first_line_model.probs_[0][0][class_index] == 1.0);
      REQUIRE(first_line_model.probs_[0][1][class_index] == 1.0);
      REQUIRE(first_line_model.probs_[0][2][class_index] == 1.0);
      REQUIRE(first_line_model.probs_[0][3][class_index] == 0.0);
      REQUIRE(first_line_model.probs_[0][4][class_index] == 1.0);
      REQUIRE(first_line_model.probs_[0][5][class_index] == 1.0);
      for (unsigned row = 1; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          REQUIRE(first_line_model.probs_[row][column][class_index] == 0.0);
        }
      }
    }
  }

  // check to make sure the Model vector contains the correct values
  //REQUIRE(models[0].probs_ == empty_model.probs_);

  //REQUIRE(has_values_grid.GetPixelsGrid()[0][0]);
  //REQUIRE(has_values_grid.GetPixelsGrid()[0][1]);
  REQUIRE(first_line_model.probs_[0][0][0] == 1.0);
  REQUIRE(first_line_model.probs_[0][1][0] == 1.0);

  GIVEN("A combined model") {
    std::map<int, double> num_class_map;
    num_class_map.insert(std::pair<int, double> (0, 2));
    Model combined_model = Trainer::CombineModels(models, classes, num_class_map);

    THEN("Require the probabilities be calculated properly") {
      int index = 0;

      REQUIRE(combined_model.probs_[0][0][index] == 0.5);
      REQUIRE(combined_model.probs_[0][1][index] == 0.5);
      REQUIRE(combined_model.probs_[0][2][index] == 0.5);
      // note that the below test case is commented out because I ran
      // into an issue of comparing equality with a repeating decimal figure
      //REQUIRE(combined_model.probs_[0][3][index] == 0.0454545455);
      REQUIRE(combined_model.probs_[0][4][index] == 0.5);
      REQUIRE(combined_model.probs_[0][5][index] == 0.5);

      for (unsigned row = 1; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          double expected = 45.0000 / 990.0000;
          REQUIRE(combined_model.probs_[row][column][index] == expected);
        }
      }
    }
  }
}

SCENARIO("Split to Models works on various inputs") {
  //purely for accessing methods
  Trainer test_trainer;

  GIVEN("An empty file") {
    std::vector<std::vector<bool>> empty_image;
    std::string file_path_empty = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_file)";
    empty_image = FileReader::CreateGrid(file_path_empty);
    std::vector<int> int_vec = {0};

    std::vector<Model> model_vector = test_trainer.SplitToModels(file_path_empty, int_vec);

    THEN("Require a model vector of 1 in length with a 0 filled model") {
      REQUIRE(model_vector.size() == 1);

      //a few random checks to make sure this calculated properly
      REQUIRE(model_vector[0].probs_[0][0][0] == 0.0);
      REQUIRE(model_vector[0].probs_[14][3][0] == 0.0);
      REQUIRE(model_vector[0].probs_[24][18][0] == 0.0);
    }
  }

  GIVEN("A non-empty file") {
    std::vector<std::vector<bool>> image;
    std::string file_path_valid = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\another_valid_file)";
    image = FileReader::CreateGrid(file_path_valid);
    std::vector<int> int_vec = {0};

    std::vector<Model> model_vector = test_trainer.SplitToModels(file_path_valid, int_vec);

    THEN("Require a model vector of 1 in length with a 0 filled model") {
      REQUIRE(model_vector.size() == 1);

      //a few random checks to make sure this calculated properly
      REQUIRE(model_vector[0].probs_[0][0][0] == 0.0);
      REQUIRE(model_vector[0].probs_[1][0][0] == 1.0);
      REQUIRE(model_vector[0].probs_[2][1][0] == 1.0);

      for (unsigned row = 3; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          REQUIRE(model_vector[0].probs_[row][column][0] == 0.0);
        }
      }
    }
  }

  GIVEN("An empty text block followed by valid text") {
    std::vector<std::vector<bool>> image;
    std::string file_path_valid = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_then_valid)";
    image = FileReader::CreateGrid(file_path_valid);
    std::vector<int> int_vec = {0, 0};

    std::vector<Model> model_vector = test_trainer.SplitToModels(file_path_valid, int_vec);

    THEN("Require a model vector of size 2") {
      REQUIRE(model_vector.size() == 2);

      for (unsigned row = 0; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          REQUIRE(model_vector[0].probs_[0][0][0] == 0.0);
        }
      }

      REQUIRE(model_vector[1].probs_[0][0][0] == 0.0);
      REQUIRE(model_vector[1].probs_[1][0][0] == 1.0);
      REQUIRE(model_vector[1].probs_[2][1][0] == 1.0);

      for (unsigned row = 3; row < IMAGE_SIZE; row++) {
        for (unsigned column = 0; column < IMAGE_SIZE; column++) {
          REQUIRE(model_vector[1].probs_[row][column][0] == 0.0);
        }
      }
    }
  }
}

SCENARIO("Find Labels tests") {
  GIVEN("A file with 3 labels") {
    Trainer test_trained;
    std::string file_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\samplelabels)";
    std::vector<int> label_vector = test_trained.FindLabels(file_path);

    THEN("Require a vector with 3 labels") {
      REQUIRE(label_vector[0] == 5);
      REQUIRE(label_vector[1] == 0);
      REQUIRE(label_vector[2] == 4);
      REQUIRE(label_vector.size() == 3);
    }
  }

  GIVEN("An empty file") {
    Trainer test_trained;
    std::string file_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_file)";
    std::vector<int> label_vector = test_trained.FindLabels(file_path);

    THEN("Require an empty vector") {
      REQUIRE(label_vector.empty());
    }
  }
}

SCENARIO("Create Num Counts tests") {
  GIVEN("A file with 3 labels") {
    Trainer test_trained;
    std::string file_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\samplelabels)";
    std::ifstream file(file_path);
    std::vector<int> num_classes;
    std::map<int, double> num_counts;

    test_trained.CreateNumCount(file, num_classes, num_counts);
    int expected_size = 3;

    THEN("Require a vector with 3 labels") {
      REQUIRE(num_classes[0] == 5);
      REQUIRE(num_classes[1] == 0);
      REQUIRE(num_classes[2] == 4);
      REQUIRE(num_classes.size() == expected_size);
    }

    THEN("Require a map with 3 pairs") {
      REQUIRE(num_counts.find(5)->second == 1);
      REQUIRE(num_counts.find(0)->second == 1);
      REQUIRE(num_counts.find(4)->second == 1);
      REQUIRE(int(num_counts.find(2)->second) == 0);
      REQUIRE(int(num_counts.find(9)->second) == 0);
      REQUIRE(num_counts.size() == expected_size);
    }
  }

  GIVEN("An empty file") {
    Trainer test_trained;
    std::string file_path = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_file)";
    std::ifstream file(file_path);
    std::vector<int> num_classes;
    std::map<int, double> num_counts;

    test_trained.CreateNumCount(file, num_classes, num_counts);

    THEN("Require a vector with no labels") {
      REQUIRE(num_classes.empty());
    }

    THEN("Require a map with no pairs") {
      REQUIRE(num_counts.empty());
    }
  }
}
