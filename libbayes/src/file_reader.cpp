//
// Created by mxdra on 10/28/2019.
//

#include "file_reader.hpp"

std::vector<std::vector<bool>> FileReader::CreateGrid(std::string file_name) {
  std::ifstream file(file_name);
  std::vector<std::vector<bool>> ret_grid;
  std::string curr_line;

  // read each line of the file
  // create a bool vector for each line that gets appended to our grid
  while (std::getline(file, curr_line)) {
    std::vector<bool> line_as_bools;

    for (char curr_char : curr_line) {
      line_as_bools.push_back(curr_char == '+' || curr_char == '#');
    }

    // if the current line has less than 28 characters, fill with blanks
    while (line_as_bools.size() < kLineLength) {
      line_as_bools.push_back(false);
    }

    ret_grid.push_back(line_as_bools);
  }

  // if the file provided has less than an even 28 mod lines of char input
  // fill the rest of the lines with blanks
  std::vector<bool> fill(kLineLength, false);

  if (ret_grid.empty()) {
    ret_grid.push_back(fill);
  }

  while (ret_grid.size() % kLineLength != 0) {
    ret_grid.push_back(fill);
  }

  return ret_grid;
}
