//Transforms text like twenty-two/twentytwo/twenty two
//or six hundred twentyfive etc to numbers like 22, 625
//Written by: Croitoru Cristian

#ifndef TEXT_TO_NUM_HGUARD
#define TEXT_TO_NUM_HGUARD
#include <string>
#include <utility>
#include <sstream>
#include <cmath>
#include <stack>
#include "token_stream.hpp"

// #define TOKEN_DEBUG_PRINT
#ifdef TOKEN_DEBUG_PRINT
#include <iostream>
#endif

#define ERROR_PARSING_NUM -2

bool has_numerical_substr(const std::string& text);

std::string sep_num_from_words(const std::string& text);

int64_t extract_numerical(const std::string& text);

int64_t text2num(const std::string& text);

// template<typename T>
// bool compare_magnitude(size_t a, size_t b){
//     T comparator;
//     return comparator((int)log10(a) + 1,(int)log10(b) + 1);
// }

size_t mag(size_t a);
int apply_error_check_pass(std::deque<size_t> & stream);
int apply_two_digit_pass(std::deque<size_t> & stream);
int apply_number_pass(std::deque<size_t> & stream);
int64_t apply_calculation_pass(std::deque<size_t> & stream);
#endif