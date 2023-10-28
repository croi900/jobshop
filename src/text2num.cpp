#include "../include/text2num/text2num.hpp"

bool has_numerical_substr(const std::string& text){
    for(auto & c : text){
        if(isdigit(c)) return true;
    }
    return false;
}

//returns a pair of two strings
//num - all the numerical charachters
//in order of their appearance
//words - all letters in order of
// their appearance, spaces are lost


//NOTE: spaces are unimportant for the
//purpose of this function, however
//do not reuse this if spaces are important
std::string sep_num_from_words(const std::string& text){
    std::string num = "";
    // std::string words = "";

    for(auto & c : text){
        if(isdigit(c)) num.push_back(c);
        // else if(isalpha(c)) words.push_back(c);
    }
    return num;
}

int64_t extract_numerical(const std::string& text){
    std::stringstream ss(text);
    int64_t ret;
    ss >> ret;
    return ret;
}

size_t mag(size_t a){
    return (size_t)log10(a) + 1;
}

int apply_error_check_pass(std::deque<size_t> & stream){
    for(size_t i = 0; i < stream.size() - 1; ++i){
        if(mag(stream[i]) == 1 && mag(stream[i+1]) == 1){
            std::cerr << "two numbers of same magnitude in section:"
                    << stream[i] << " " << stream[i+1] << '\n'; 
            return ERROR_PARSING_NUM;
        }
    }
    return 0;
}


int apply_two_digit_pass(std::deque<size_t> & stream){
    for(size_t i = 0; i < stream.size() -1 ; ++i){
        if(mag(stream[i]) == 2 && mag(stream[i+1]) == 1){
            stream[i] += stream[i+1];
            stream.erase(stream.begin() + i + 1);
        }
    }

    return 0;
}

int apply_number_pass(std::deque<size_t> & stream){
    for(size_t i = 1; i < stream.size() ; ++i){
        if(mag(stream[i]) > 3){
            if(mag(stream[i-1]) <= 3){
                stream[i-1] = stream[i-1] * stream[i];
                stream.erase(stream.begin() + i );
            }else{
                 std::cerr << "error trying to parse section:"
                              << stream[i-1] << " " << stream[i] << '\n'; 
                return ERROR_PARSING_NUM;
            }
        }else if(mag(stream[i]) == 3 && mag(stream[i-1]) <=2){
            if( mag(stream[i-1]) == 2 && 
                stream[i-1]%10 == 0){
                std::cerr << "error trying to parse section:"
                            << stream[i-1] << " " << stream[i] << '\n'; 
                return ERROR_PARSING_NUM;
            }

            stream[i-1] = stream[i-1] * stream[i];
            stream.erase(stream.begin() + i );
        }
    }
    return 0;
}
//assumes the stream is a descending order
//of numbers representig the different orders
//of the final parsed number

int64_t apply_calculation_pass(std::deque<size_t> & stream){
    int64_t calc = 0;
    size_t maxmag = mag(stream[0]);
    for(size_t i = 0; i < stream.size(); ++i){
        if(mag(stream[i]) >= maxmag){
            std::cerr   << "error calculating nr at section:"
                        <<  stream[i] << "\n"; 
            return ERROR_PARSING_NUM;
        }else{
            maxmag = std::max(maxmag, mag(stream[i]));
            calc += stream[i];
        }
    }

    return calc;
}


int64_t apply_calculation_pass2(std::deque<size_t> & stream){
    int64_t calc = 1;
    size_t maxmag = mag(stream[0]);
    for(size_t i = 0; i < stream.size(); ++i){
        
        if(calc < stream[i]){
            calc *= stream[i];
        }else if(calc > stream[i]){
            calc += stream[i];
        }
    }

    return calc;
}

int64_t apply_calculation_pass(TokenStream & stream){

    int64_t calc = 0;
    int64_t toadd = 1;
    auto tok = stream.next_token();

    auto cmag = 900000;

    std::stack<size_t> mul;
    // mul.push(tok);
    // tok = stream.next_token();
    while(tok != TOKEN_END){
        if(mul.empty()){
            mul.push(tok);
        }else if((mag(tok) > mag(mul.top()))){
            mul.push(tok);
        }else if(mag(tok) < mag(mul.top())){
            while(!mul.empty()){
                toadd *= mul.top();
                mul.pop();
            }
            calc += toadd;
            toadd = 1;
            mul.push(tok);
        }

        tok = stream.next_token();
    }

    toadd = 1;
    while(!mul.empty()){
        toadd *= mul.top();
        mul.pop();
    }
    calc += toadd;

    return calc;
}

int64_t text2num(const std::string& text){

    // auto nw_pair = 
    auto num = sep_num_from_words(text);
    auto words = ' ' + text + ' ';

    //numerical characters take priority
    if(has_numerical_substr(num))
        return extract_numerical(num);
    
    TokenStream ts(words);
    if(ts.token_stream.empty()) return 0;

#ifdef TOKEN_DEBUG_PRINT
    for(auto & e : ts.token_stream){
        std::cout << e << ' ';
    }std::cout << std::endl;
#endif

    if(apply_error_check_pass(ts.token_stream) == ERROR_PARSING_NUM){
        return ERROR_PARSING_NUM;
    }
    // if(apply_two_digit_pass(ts.token_stream) == ERROR_PARSING_NUM){
    //     return ERROR_PARSING_NUM;
    // }
    // if(apply_number_pass(ts.token_stream) == ERROR_PARSING_NUM){
    //     return ERROR_PARSING_NUM;
    // }
    


    // int64_t calculated = 0;
#ifdef TOKEN_DEBUG_PRINT
    for(auto & e : ts.token_stream){
        std::cout << e << ' ';
    }std::cout << std::endl;
#endif


// #ifdef TOKEN_DEBUG_PRINT
//     while(tok != TOKEN_END){
//         std::cout << tok << std::endl;
//         tok = token_stream.next_token();
//     }    
// #endif

    return apply_calculation_pass2(ts.token_stream);
}
