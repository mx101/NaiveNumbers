//
// Created by mxdra on 10/28/2019.
//
#include <iostream>

#include "catch.hpp"
#include "file_reader.hpp"

SCENARIO("File Reader Cases") {
  unsigned line_length = 28;

  auto *reader = new std::filebuf;

  GIVEN("A bad file path") {
    std::string filename = "bad file path";
    std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

    THEN("Require a 28x28 vector filled with falses") {
      std::vector<std::vector<bool>> expected;

      while (expected.size() < line_length) {
        std::vector<bool> false_filled_vector(line_length, false);
        expected.push_back(false_filled_vector);
      }

      REQUIRE(expected == actual);
    }
  }

  GIVEN("An empty file") {
    std::string filename = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_file)";
    std::istream line_istream(reader);
    std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

    THEN("Require a 28x28 vector filled with falses") {
      std::vector<std::vector<bool>> expected;

      while(expected.size() < line_length) {
        std::vector<bool> false_filled_vector(line_length, false);
        expected.push_back(false_filled_vector);
      }

      REQUIRE(expected == actual);
    }
  }

  GIVEN("A file with one line") {
    std::string filename = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\single_line_file)";
    std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

    THEN("Require a 28x28 2D vector with one vector not filled with false") {
      std::vector<std::vector<bool>> expected;

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

      REQUIRE(first_line == actual[0]);

      expected.push_back(first_line);

      while(expected.size() < line_length) {
        std::vector<bool> false_filled_vector(line_length, false);
        expected.push_back(false_filled_vector);
      }

      REQUIRE(expected == actual);

      // additional position checks
      REQUIRE(actual[0][0]);
      REQUIRE(actual[0][1]);
      REQUIRE(actual[0][2]);
      REQUIRE_FALSE(actual[0][3]);
      REQUIRE(actual[0][4]);
      REQUIRE(actual[0][5]);
      REQUIRE_FALSE(actual[0][6]);
      REQUIRE_FALSE(actual[0][7]);

      // 1 stands for true, 0 for false
      // expected vector:
      // 1 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
      // next 27 lines are false
    }
  }

  GIVEN("A file with multiple valid sequential lines") {
    std::string filename = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\valid_file)";
    std::istream line_istream(reader);
    std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

    THEN("Require CreateGrid returns as expected with various filled positions") {
      std::vector<std::vector<bool>> expected;

      std::vector<bool> first_line;
      first_line.push_back(true);
      first_line.push_back(true);
      first_line.push_back(false);
      first_line.push_back(true);
      while (first_line.size() < line_length) {
        first_line.push_back(false);
      }
      expected.push_back(first_line);


      std::vector<bool> second_line;
      second_line.push_back(false);
      second_line.push_back(true);
      second_line.push_back(false);
      second_line.push_back(true);
      while (second_line.size() < line_length) {
        second_line.push_back(false);
      }
      expected.push_back(second_line);

      while(expected.size() < line_length) {
        std::vector<bool> false_filled_vector(line_length, false);
        expected.push_back(false_filled_vector);
      }

      REQUIRE(expected == actual);
    }
  }

  GIVEN("A file with multiple valid sequential lines starting with a blank line returns properly") {
    std::string filename = R"(C:\Users\mxdra\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\another_valid_file)";

    std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

    THEN("Require a 28x28 2D vector with a false-filled first vector and rest as expected") {
      std::vector<std::vector<bool>> expected;

      std::vector<bool> first_line;
      while(first_line.size() < line_length) {
        first_line.push_back(false);
      }
      expected.push_back(first_line);


      std::vector<bool> second_line;
      second_line.push_back(true);
      second_line.push_back(true);
      second_line.push_back(false);
      second_line.push_back(true);
      while (second_line.size() < line_length) {
        second_line.push_back(false);
      }
      expected.push_back(second_line);


      std::vector<bool> third_line;
      third_line.push_back(false);
      third_line.push_back(true);
      third_line.push_back(false);
      third_line.push_back(true);
      while (third_line.size() < line_length) {
        third_line.push_back(false);
      }
      expected.push_back(third_line);

      while(expected.size() < line_length) {
        std::vector<bool> false_filled_vector(line_length, false);
        expected.push_back(false_filled_vector);
      }

      REQUIRE(expected == actual);
    }
  }
}

TEST_CASE("empty file size testing") {
  auto *reader = new std::filebuf;
  std::string filename = R"(D:\Users\Michael\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\empty_file)";
  std::istream line_istream(reader);
  std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

  int expected_size = 28;

  REQUIRE(actual.size() == expected_size);
  delete reader;
}

TEST_CASE("Filled file size testing") {
  auto *reader = new std::filebuf;
  std::string filename = R"(D:\Users\Michael\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\filled_shaded)";

  std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

  int expected_size = 28;

  REQUIRE(actual.size() == expected_size);
  delete reader;
}

TEST_CASE("Long file size testing") {
  std::string filename = R"(D:\Users\Michael\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\long_file)";
  std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

  int expected_size = 35;
  int line_length = 28;

  REQUIRE(actual.size() == expected_size);

  std::vector<bool> expected = {false, false, false, false, true, true, false, true};
  while (expected.size() < line_length) {
    expected.push_back(false);
  }

  int expected_idx = 33;

  REQUIRE(expected == actual[expected_idx]);
}

TEST_CASE("Longer file size testing") {
  std::string filename = R"(D:\Users\Michael\CLionProjects\naive-numbers-mx101\libbayes\test\resources\file_reader_test_files\another_long_file)";

  std::vector<std::vector<bool>> actual = FileReader::CreateGrid(filename);

  int expected_size = 106;
  int line_length = 28;

  REQUIRE(actual.size() == expected_size);

  std::vector<bool> expected = {false, true, true, false, true};
  while (expected.size() < line_length) {
    expected.push_back(false);
  }

  int expected_idx = 71;

  REQUIRE(expected == actual[expected_idx]);
}