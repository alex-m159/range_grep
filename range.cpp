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
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <optional>
#include <string.h>
#include <algorithm>
#include <future>
#include <queue>
#include <aio.h>
#include <csignal>


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

int read_data(std::ifstream* __restrict__ file_stream, size_t buffersize, int start_of_line, int curr, char* __restrict__ curr_buff, char* __restrict__ next_buff) {
    int saved_len = (curr - start_of_line);
    strncpy(next_buff, &curr_buff[start_of_line], saved_len);
    file_stream->read( &next_buff[saved_len], buffersize - saved_len - 1);
    std::swap(curr_buff, next_buff);
    return saved_len;
}

void write_plain_data(char* buf, char* pre, int byte_count, std::vector<std::pair<int, int>> matches) {
    printf("%.*s", byte_count, buf);
}

// pre for preallocated, since I don't want to allocate lots of memory for every write
void write_color_data(char* buf, char* pre, int byte_count, std::vector<std::pair<int, int>> matches) {
    
    int q_size = static_cast<int>(matches.size());
    int buf_temp = 0;
    int pre_temp = 0;
    // will transfer the chars before the match and the match itself into pre,
    // then after the loop any remaining chars will be transferred to pre.
    
    for(int i = 0; i < q_size; i++) {
        
        auto m = matches[i];
        int fst = m.first;
        int snd = m.second;
        int n_before = fst - buf_temp;
        int n = snd - fst;
        
        // copy the segment before the match
        strncpy(&pre[pre_temp], &buf[buf_temp], n_before);
        pre_temp += n_before;
        buf_temp += n_before;
        
        // add the color code
        pre[pre_temp] = '\033';
        pre[pre_temp+1] = '[';
        pre[pre_temp+2] = '1';
        pre[pre_temp+3] = ';';
        pre[pre_temp+4] = '3';
        pre[pre_temp+5] = '1';
        pre[pre_temp+6] = 'm';
        pre_temp += 7;
        
        // copy the match
        strncpy(&pre[pre_temp], &buf[buf_temp], n);
        buf_temp += n;
        pre_temp += n;
        
        // end the color
        pre[pre_temp] = '\033';
        pre[pre_temp+1] = '[';
        pre[pre_temp+2] = '0';
        pre[pre_temp+3] = 'm';
        pre_temp += 4;
    }
    
    int tail = byte_count - buf_temp;
    strncpy(&pre[pre_temp], &buf[buf_temp], tail);
    printf("%.*s", static_cast<int>(byte_count + q_size*11), pre);
    
}


/*
Will maybe be callback for Linux aio
void callback(union sigval sig_val) {
    printf("inside callback\n");
}
*/

int main(int argc, char* argv[]){

    if(argc != 4) {
        std::cout << "usage: "<< argv[0] <<" <lowest number> <highest number> <file>" << std::endl;
        return 0;
    }
    
    // -o flag doesn't do anything yet
    std::string print_only_match_short = "-o";
    std::string print_only_match = "--only-matching";
    bool only_matching = false;
    std::string print_color = "--color";
    bool color = false;

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], print_only_match.c_str()) == 0 || strcmp(argv[i], print_only_match_short.c_str()) == 0) {
            only_matching = true;
        }
        if(strcmp(argv[i], print_color.c_str()) == 0) {
            color = true;
        }
    }

    void (*write_line)(char*, char*, int, std::vector<std::pair<int, int>>);
    if(color)
        write_line = write_color_data;
    else
        write_line = write_plain_data;            

    int start_of_line;
    int dist;
    int prev;
    int curr;
    bool print_this_line = false;
    std::string _lo;
    std::string _hi;
    
    std::string file;
    std::ifstream file_stream;
    constexpr size_t buffersize = 4026 * 32;
    std::vector<std::pair<int, int>> matches;
    
    _lo.assign(argv[1]);
    _hi.assign(argv[2]);
    file.assign(argv[3]);
    
    file_stream.open(file);
    
    if(file_stream.fail()) {
        std::cout << "file could not be opened: " << file << std::endl;
        std::exit(1);
    }

    
    //int stdout_fd = fileno(stdout);

    char curr_buff[buffersize];
    char next_buff[buffersize];
    // Add extra space for the console color code characters
    char pre[ buffersize + 300 ];
    prev = -1;
    dist = -1;
    curr = 0;


    file_stream.read(curr_buff, buffersize);
        
    if( file_stream.fail() && !file_stream.eof() ) {
        std::cout << "exiting due to file stream error" << std::endl;
        
        std::exit(1);
    }
    start_of_line = 0;

    while( file_stream.gcount() > 0 ){
        std::streamsize charsread = file_stream.gcount();
        for(int i = 0; i < static_cast<int>(charsread); i++) {
            
            if(is_num(curr_buff[curr]) && curr_buff[curr] != '0' && dist == -1 ){
                // we must be at the beginning of a number
                dist = curr;
            } else if( !is_num(curr_buff[curr]) && is_num(curr_buff[prev]) ) {
                // we're at the end of a number
                // print everything from dist to prev
                // mark this line as needing to be printed
                if( in_range(curr_buff, dist, prev+1, _lo, _hi) ) {

                    print_this_line = true;
                    // we just want to store sart/end offsets into the line, not the whole buffer
                    matches.push_back( std::pair<int, int>(dist - start_of_line, prev+1 - start_of_line) );
                }
                prev = curr;
                dist = -1;
            }
            if( curr_buff[curr] == '\n' && print_this_line) {
                
                
                int byte_count = (curr - start_of_line)+1;               
                char* start = &curr_buff[start_of_line];
                
                write_line(start, pre, byte_count, matches);
                matches.clear();
                print_this_line = false;
            }
            if( curr_buff[prev] == '\n' ) {
                start_of_line = curr;
                
            }
            prev = curr;
            curr++;
        }
        file_stream.clear();
       
        if(curr_buff[curr] != '\n') {
            // our buffer ends in the middle of a line, so we have to save that in case 
            // we need to print it out.
            
            // we're employihg a "rolling" buffer strategy. We'll copy the carried-over
            // line data into the "next" buffer then fill the next buffer with new data.
            // Then we can avoid copying the carried-over line data twice.
            /*
                __________________   __________________
                |_________________|  |_________________| 

                                     __________________   __________________
                                     |_________________|  |_________________| 

                                                          __________________   __________________
                                                          |_________________|  |_________________| 

            */
            //  It's a small optimization, but it made a difference in my testing.
            int saved_len = read_data(&file_stream, buffersize, start_of_line, curr, curr_buff, next_buff);

            curr = saved_len;
            if( dist != -1 )
                dist = dist - start_of_line;
            prev = curr - 1;
            start_of_line = 0;
            
        } else {
            file_stream.read(curr_buff, buffersize);
            curr = 0;
            dist = -1;
            prev = curr - 1;
            start_of_line = 0;
            print_this_line = false;
        }
        
    }
    return 0;
}



