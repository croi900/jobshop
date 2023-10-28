#include "../include/parse_utils.hpp"


void conv_to_parsable(std::string& line){
    std::string new_str;
    std::transform( line.begin(), line.end(),
                    line.begin(),tolower);

    for(auto & c : line){
        if(isalnum(c))
            new_str.push_back(c);
    }
    line = new_str;
}

//will extract words from first letter encountered
//untill the first nonalnum nonwhitespace charachter
std::string extract_first_words(const std::string& text){
    std::string ret = "";
    bool found_alpha = false;
    for(auto & c: text){
        if(isalpha(c))
            found_alpha = true;
        if(found_alpha && !(isalnum(c) || isspace(c)))
            break;
        if(found_alpha)
            ret.push_back(c);
    }
    return ret;
}

//same as above, but stops once a charachter read
//after first letter is found does not fulfill the
//predicarte
std::string extract_first_words(const std::string& text, std::function<bool(char)> pred){
    std::string ret = "";
    bool found_alpha = false;
    for(auto & c: text){
        if(isalpha(c))
            found_alpha = true;
        if(found_alpha && !(pred(c)))
            break;
        if(found_alpha)
            ret.push_back(c);
    }
    return ret;
}

//will extract words from first letter encountered
//untill the first nonalnum nonwhitespace charachter
//in reverse

std::string extract_last_words(const std::string& text){
    std::string ret = "";
    bool found_alpha = false;
    for(int i = text.length() - 1; i >= 0; i--){
        char c= text[i];
        if(isalpha(c))
            found_alpha = true;
        if(found_alpha && !(isalnum(c)|| isspace(c)))
            break;
        if(found_alpha)
            ret.push_back(c);
    }
    std::reverse(ret.begin(),ret.end());

    return ret;
}

//extracts the first POSITIVE number
//found in text
//the number must be less than 2^60
uint64_t extract_first_num(const std::string& text){
    int ret = 0;
    bool found_num = false;
    for(auto & c: text){
        if(isdigit(c))
            found_num = true;
        if(found_num && !(isdigit(c)))
            break;
        if(found_num){
            if(ret >= 1LL<<60) break;
            ret = ret*10 + (c-'0');
        }
    }
    return ret;
}

bool has_chars(std::string& text){
    // bool ret = false;
    for(auto& c : text){
        if(!isspace(c)){
            return true;
        }
    }

    return false;
}

bool begins_with_number(const std::string& text){
    bool begins = true;
    for(auto & c : text){
        if(isspace(c)) continue;
        if(isdigit(c)){
            break;
        }
        if(!isdigit(c)){
            begins = false;
            break;
        }
    }

    return begins;
}

std::pair<std::string,std::string> 
separate_by_char_trim(std::string& text,char sep){
    bool found_letter = false, found_colon = false;
    std::string pre = "", post = "";

    for(int i = text.length() - 1; i >= 0; --i){
        char c = text[i];
        if(isalnum(c))
            found_letter = true;
        if(found_letter && !found_colon)
            pre += c;
        if(c == sep){
            found_colon = true;
            continue;
        }
        if(found_colon)
            post += c;
    }
    std::reverse(pre.begin(), pre.end());
    std::reverse(post.begin(), post.end());
    
    //reversed because reading from reverse
    return std::make_pair(post, pre);

}


std::string get_filename(const std::string& path){
    std::string ret = "";
    for(int i = path.length() - 1; i >= 0; --i){
        if(path[i] == '\\' || path[i] == '/')
            break;

        ret.push_back(path[i]);
    }

    std::reverse(ret.begin(),ret.end());
    return ret;
}