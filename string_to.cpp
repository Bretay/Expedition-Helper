#include "string_to.h"

#include <sstream>

bool StringToInt(std::string value, int& int_to_store) {
  std::stringstream converter(value);
  converter.exceptions(std::ios_base::failbit);
  try {
    converter >> int_to_store;
  } catch (std::ios_base::failure f) {
    std::cout << "Couldn't parse integer value."
              << std::endl << std::endl;
    return false;
  }
  return true;
}

bool StringToDouble(std::string value, double& double_to_store) {
  std::stringstream converter(value);
  converter.exceptions(std::ios_base::failbit);
  try {
    converter >> double_to_store;
  } catch (std::ios_base::failure f) {
    std::cout << "Couldn't parse floating point value."
              << std::endl << std::endl;
    return false;
  }
  return true;
}

bool StringToChar(std::string value, char& char_to_store) {
  std::stringstream converter(value);
  converter.exceptions(std::ios_base::failbit);
  try {
    converter >> char_to_store;
  } catch(std::ios_base::failure f) {
    std::cout << "Couldn't parse character value."
              << std::endl << std::endl;
    return false;
  }
  return true;
}
