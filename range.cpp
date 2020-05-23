#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <optional>
//#include <istream>
/*
struct Rope{
    std::vector arr(4096, char);
    std::optional<Rope*> next = std::nullopt;
}
*/
    
std::vector<std::pair<int, int>> find_numerics(std::string in){
    int start = -1;
    int end = -1;
    int first = 0;
    int last = in.length();
    int curr = first;
    std::vector<std::pair<int, int>> indices;
    indices.reserve(last);
    
    while(curr < last){
        start = -1;
        end = -1;
        while(curr < last && !(48 <= in[curr] && in[curr] <= 57)){
            curr++;
        }
        // post-condition: curr is pointing to an ASCII numeric char and may be past the end of the string
        
        // we reached the end of the string and there's no number, so we just exit the loop
        if(curr >= last)
            break;
        start = curr;
        while(curr < last && (48 <= in[curr] && in[curr] <= 57) ){
            curr++;
        }
        // post-condition: curr is not pointing to ASCII numeric char and may be past the end of the string
        end = curr;
        indices.push_back(std::pair<int, int>(start, end));
    }
    return indices;
}

std::vector<std::string> extract_substrings(std::vector<std::pair<int, int>> indices, std::string s) {
    std::vector<std::string> strs;
    strs.reserve(indices.size());
    
    for(int i = 0; i < static_cast<int>(indices.size()); i++){
        std::string sub = s.substr(indices[i].first, indices[i].second - indices[i].first);
        strs.push_back(sub);
    }
    return strs;
}




bool is_num(char c){
    return (48 <= c && c <= 57);
}


bool in_range(std::string check, std::string low, std::string hi){
    
    if(check.length() < low.length() || hi.length() < check.length() )
        return false;
    
    // if check is between the length of low and hi, then it must also have 
    // a value in between as well.
    if(low.length() < check.length() && check.length() < hi.length() )
        return true;

    // this will only run if check is equal in length to either low or hi
    if(low.length() == check.length()){
        for(int i = 0; i < static_cast<int>(check.length()); i++){
            if(low[i] != check[i]) {
                if( check[i] < low[i]  ) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    } else if(hi.length() == check.length()) {
        for(int i = 0; i < static_cast<int>(check.length()); i++){
            if(hi[i] != check[i]) {
                if( hi[i] < check[i]  ) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
    return true;
}




int main(int argc, char* argv[]){

    if(argc != 4) {
        std::cout << "usage: "<< argv[0] <<" <lowest number> <highest number> <file>" << std::endl;
        return 0;
    }
    
    
    int start_of_line;
    int dist;
    int prev;
    int curr;
    bool print_this_line;
    std::string lo;
    std::string hi;
    std::string file;
    std::ifstream file_stream;
    constexpr size_t buffersize = 4026 * 32;
    
    lo.assign(argv[1]);
    hi.assign(argv[2]);
    file.assign(argv[3]);
    //std::cout << "file: " << file << std::endl;
    file_stream.open(file);
    
    if(file_stream.fail()) {
        std::cout << "file could not be opened: " << file << std::endl;
        std::exit(1);
    }
    
    std::vector<char> text(buffersize);
    prev = -1;
    dist = -1;
    curr = 0;
    
    file_stream.read(text.data(), buffersize);
        
    if( file_stream .fail() && !file_stream.eof() ) {
        std::cout << "exiting due to file stream error" << std::endl;
        
        std::exit(1);
    }
    start_of_line = 0;

    while( file_stream.gcount() > 0 ){
        //std::cout << "inside while" << std::endl;
        std::streamsize charsread = file_stream.gcount();
        for(int i = 0; i < static_cast<int>(charsread); i++) {
            curr++;
            if(is_num(text[curr]) && text[curr] != '0'){
                if(dist == -1 ) {
                    // we must be at the beginning of the line
                    dist = curr;
                }
                
            } else if( !is_num(text[curr]) && is_num(text[prev]) ) {
                // we're at the end of a number
                // print everything from dist to prev
                // mark this line as needing to be printed
                std::string match(text.begin() + dist, text.begin() + prev + 1);
                /*                
                if(dist == -1)
                    std::cout << "dist: -1" << std::endl;
                else
                    std::cout << "dist: " << text[dist] << std::endl;
                std::cout << "prev: " << text[prev] << std::endl;
                std::cout << "curr: " << text[curr] << std::endl;
                */
                //std::cout << "match: " << match.c_str() << std::endl;
                //std::cout << "low: " << lo << std::endl;
                //std::cout << "high: " << hi << std::endl;
                
                if( in_range(match, lo, hi) ) {
                    //std::cout << "match: " << match.c_str() << std::endl;
                    //std::cout << "in range" << std::endl;
                    print_this_line = true;
                }
                prev = curr;
                dist = -1;
            }
            if( text[curr] == '\n' && print_this_line) {
                // end of the line. Print if we need to...
                std::string line(&text[start_of_line], &text[curr]);
                std::cout << "line: " << line << std::endl;
                print_this_line = false;
            }
            if( text[prev] == '\n' ) {
                start_of_line = curr;
                
            }
            prev = curr;
        }
        file_stream.clear();
       
        if(text[curr] != '\n') {
            // our buffer ends in the middle of a line, so we have to save that 
            std::vector<char> temp(text.begin() + start_of_line, text.begin() + curr);
            std::move(temp.begin(), temp.end(), text.data());
            file_stream.read(text.data() + temp.size(), buffersize - temp.size());
            curr = curr - start_of_line;
            if( dist != -1 )
                dist = dist - start_of_line;
            prev = curr - 1;
            start_of_line = 0;
            print_this_line = false;
        } else {
            file_stream.read(text.data(), buffersize);
            curr = 0;
            dist = -1;
            prev = curr - 1;
            start_of_line = 0;
            print_this_line = false;
        }
        
    }
    
    return 0;
}




































