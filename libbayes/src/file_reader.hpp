//
// Created by mxdra on 10/28/2019.
//

#ifndef NAIVEBAYES_LIBBAYES_SRC_FILE_READER_HPP_
#define NAIVEBAYES_LIBBAYES_SRC_FILE_READER_HPP_

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

class FileReader {
 public:
  /// Generates a 2D vector given a file location
  static std::vector<std::vector<bool>> CreateGrid(std::string);
 private:
  const static unsigned kLineLength = 28;

};

#endif //NAIVEBAYES_LIBBAYES_SRC_FILE_READER_HPP_
