#ifndef STRING_TO_H
#define STRING_TO_H

#include <iostream>

/*
 *  Attempts to convert a string to an integer value. If the operation fails,
 *  it returns false. If it succeeds, function returns true.
 *  @param std::string value - the string to attempt conversion with
 *  @param int& int_to_store - the integer to store the value in at the end.
 *                             should be unchanged if operation failed.
 *  @return                  - true if successful, false if it fails
 */
bool StringToInt(std::string value, int& int_to_store);

/*
 *  Attempts to convert a string to an float value. If the operation fails,
 *  it returns false. If it succeeds, function returns true.
 *  @param std::string value       - the string to attempt conversion with
 *  @param double& double_to_store - the float to store the value in at the end.
 *                                   should be unchanged if operation failed.
 *  @return                        - true if successful, false if it fails
 */
bool StringToDouble(std::string value, double& double_to_store);

bool StringToChar(std::string value, char& char_to_store);

#endif /* STRING_TO_H */
