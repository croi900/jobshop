#include "../include/text2num/token_stream.hpp"

TokenStream::TokenStream(const std::string& to_parse)
{
    std::string unprocessed = to_parse;
    using token = std::pair<size_t, size_t>;
    std::vector<token> tok_pos;
    for(auto& def : predef_tokens){
        auto it = unprocessed.find(def.first);
        while(it != std::string::npos){
            std::fill_n(unprocessed.begin() + it, def.first.size(), '#');

            tok_pos.push_back({it,def.second});
            it = unprocessed.find(def.first,it+1);
        }
    }

    std::sort(tok_pos.begin(),tok_pos.end(),[](auto& a, auto& b){
        return a.first < b.first;
    });

    while(tok_pos.empty() == false){
        token_stream.push_front(tok_pos.back().second);
        tok_pos.pop_back();
    }
}

TokenStream::TokenStream(/* args */)
{
}

TokenStream::~TokenStream()
{
}

size_t TokenStream::next_token(){
    if(token_stream.empty()) return TOKEN_END;

    auto next = token_stream.front();
    token_stream.pop_front();
    return next;
}