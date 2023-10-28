#ifndef PARSE_UTILS_HGUARD
#define PARSE_UTILS_HGUARD
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <functional>
#include <ctype.h>
#include <sstream>
#include <utility>
void conv_to_parsable(std::string& line);
//will extract words from first letter encountered
//untill the first nonalnum nonwhitespace charachter
std::string extract_first_words(const std::string& text);


//same as above, but stops once a charachter read
//after first letter is found does not fulfill the
//predicarte
std::string extract_first_words(const std::string& text, std::function<bool(char)> pred);
//will extract words from first letter encountered
//untill the first nonalnum nonwhitespace charachter
//in reverse

std::string extract_last_words(const std::string& text);

//extracts the first POSITIVE number
//found in text
//the number must be less than 2^60
uint64_t extract_first_num(const std::string& text);

bool has_chars(std::string& text);

bool begins_with_number(const std::string& text);

std::pair<std::string,std::string> 
separate_by_char_trim(std::string& text,char sep);

std::string get_filename(const std::string& path);

template<typename T>
typename T::size_type levdist(const T &source, const T &target) {
    if (source.size() > target.size()) {
        return levdist(target, source);
    }

    using TSizeType = typename T::size_type;
    const TSizeType min_size = source.size(), max_size = target.size();
    std::vector<TSizeType> lev_dist(min_size + 1);

    for (TSizeType i = 0; i <= min_size; ++i) {
        lev_dist[i] = i;
    }

    for (TSizeType j = 1; j <= max_size; ++j) {
        TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
        ++lev_dist[0];

        for (TSizeType i = 1; i <= min_size; ++i) {
            previous_diagonal_save = lev_dist[i];
            if (source[i - 1] == target[j - 1]) {
                lev_dist[i] = previous_diagonal;
            } else {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return lev_dist[min_size];
}

#endif