#include "range.hpp"

bool is_num(char c){
    return (48 <= c && c <= 57);
}


bool in_range(const char* __restrict__ check, int start, int end, std::string low, std::string hi){
    // unsigned to avoid compiler warning
    unsigned int len = end - start;
    if( len < low.length() || hi.length() < len )
        return false;
    
    // if len is between the length of low and hi, then it must also have 
    // a value in between as well.
    if(low.length() < len && len < hi.length() )
        return true;

    // this will only run if check is equal in length to either low or hi
    int ret;
    if(low.length() == len) {
        ret = strncmp(low.c_str(), &check[start], len);
    } else {
        ret = strncmp(&check[start], hi.c_str(), len);
    }

    if(ret < 0)
        return true;
    else
        return false;
    
}


