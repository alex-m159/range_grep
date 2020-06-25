// SPDX-License-Identifier: MIT
/*

Copyright (c) 2020 Alex Markley

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "range.hpp"
#include <iostream>

bool is_num(char c){
    return (48 <= c && c <= 57);
}

/**
 * Precondition:the value at check[start] is the beginning of an ASCII-encoded number with no leading zeros
 */
bool in_range(const char* __restrict__ check, int start, int end, std::string low, std::string hi){
    // unsigned to avoid compiler warning
    int len = end - start + 1;
    int low_len = static_cast<int>(low.length());
    int hi_len = static_cast<int>(hi.length());
    if( len < low_len || hi_len < len )
        return false;
    
    // if len is between the length of low and hi, then it must also have 
    // a value in between as well.
    if(low_len < len && len < hi_len )
        return true;
    // this will only run if check is equal in length to either low or hi
    int ret1 = 0;
    int ret2 = 0;
    if(low_len == hi_len && low_len == len) {
        ret1 = strncmp(low.c_str(), &check[start], len);
        ret2 = strncmp(&check[start], hi.c_str(), len);
    } else if(low_len == len) {
        ret1 = strncmp(low.c_str(), &check[start], len);
    } else {
        ret1 = strncmp(&check[start], hi.c_str(), len);
    }

    if(ret1 <= 0 && ret2 <= 0)
        return true;
    else
        return false;
    
}


