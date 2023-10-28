#ifndef TOKEN_STREAM_HGUARD
#define TOKEN_STREAM_HGUARD

//class will not be split into hpp and cpp
//untill further notice
#include <string>
#include <utility>
#include <deque>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <iostream>

#define TOKEN_END -2

typedef size_t Token;

class TokenStream
{
private:
    //dirty ugly trick, SHOULD 100% NOT BE HERE
    //but the linker is killing me about redefinitions
    //and it's only used here so c'est la vie
    std::unordered_map<std::string,int> 
    predef_tokens = {
        {" zero " , 0},
        {" one " , 1},
        {" two " , 2},
        {" three " , 3},
        {" four " , 4},
        {" five " , 5},
        {" six " , 6},
        {" seven " , 7},
        {" eight " , 8},
        {" nine " , 9},
        {" ten " , 10},

        {" eleven " , 11},
        {" twelve " , 12},
        {" thirteen " , 13},
        {" fourteen " , 14},
        {" fifteen " , 15},
        {" sixteen " , 16},
        {" seventeen " , 17},
        {" eighteen " , 18},
        {" nineteen " , 19},

        {" twenty " , 20},
        {" thirty " , 30},
        {" fourty " , 40},
        {" fifty " , 50},
        {" sixty " , 60},
        {" seventy " , 70},
        {" eighty " , 80},
        {" ninety " , 90},

        {" hundred " , 100},
        {" thousand " , 1000},
        {" million " , 1000000},
        {" billion " , 1000000000},

    };

public:
    std::deque<std::size_t> token_stream;

    size_t next_token();
    
    TokenStream( const std::string& unprocessed);
    TokenStream(/* args */);
    ~TokenStream();
};


#endif