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
#include <algorithm>
#include <vector>
#include <assert.h>
#include <iostream>
#include "range.cpp"


bool test_is_num(){
    std::vector<bool> res = {
        is_num('1'),
        is_num('2'),
        is_num('3'),
        is_num('4'),
        is_num('5'),
        is_num('6'),
        is_num('7'),
        is_num('8'),
        is_num('9')
    };
    
    assert(std::find(res.begin(), res.end(), false) == res.end());
    assert(is_num('/') == false);
    assert(is_num(':') == false);
    assert(is_num(';') == false);
    assert(is_num('.') == false);
    assert(is_num('A') == false);
    assert(is_num('a') == false);
    return true;
}

bool test_in_range() {
    std::string low = "11";
    std::string high = "1000001";


    assert(in_range("abcd1234efgh", 4, 7, low, high) == true);
    /* In range */
    assert(in_range("abcd12efgh", 4, 5, low, high) == true);
    /* In range */    
    assert(in_range("abcd1000000efgh", 4, 10, low, high) == true);

    /* Too low */
    assert(in_range("abcd10efgh", 4, 5, low, high) == false);
       
    /* Too high */
    assert(in_range("abcd1000002efgh", 4, 10, low, high) == false);

    /* Tests with single digit */
    assert(in_range("abcd9efgh", 4, 4, low, high) == false);

    assert(in_range("abcd9efgh", 4, 4, "8", high) == true);

    assert(in_range("abcd8efgh", 4, 4, "8", high) == true);

    assert(in_range("abcd7efgh", 4, 4, "8", high) == false);

    /* Non-number characters */
    assert(in_range("abcdefghjk", 4, 10, low, high) == false);
    return true;
}

bool test_in_range2() {
    std::string low  = "20001";
    std::string high = "50001";

    assert(in_range("abcd1234efgh", 4, 7, low, high) == false);

    assert(in_range("abcd10000efgh", 4, 8, low, high) == false);
    
    assert(in_range("abcd19999efgh", 4, 8, low, high) == false);
    
    assert(in_range("abcd20000efgh", 4, 8, low, high) == false);

    assert(in_range("abcd20001efgh", 4, 8, low, high) == true);
    
    assert(in_range("abcd49999efgh", 4, 8, low, high) == true);

    assert(in_range("abcd50000efgh", 4, 8, low, high) == true);

    assert(in_range("abcd50001efgh", 4, 8, low, high) == true);

    assert(in_range("abcd50002efgh", 4, 8, low, high) == false);
    
    return true;
}

int main() {
    test_is_num();
    test_in_range();
    test_in_range2();

    return 0;
}
