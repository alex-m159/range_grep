#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

std::vector<std::pair<int, int>> find_numerics(std::string in){
    int start = -1;
    int end = -1;
    int first = 0;
    int last = in.length();
    int curr = first;
    std::vector<std::pair<int, int>> indices;
    indices.reserve(last);
    
    int* somenum = new int;
    
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
    
    for(int i = 0; i < indices.size(); i++){
        std::string sub = s.substr(indices[i].first, indices[i].second - indices[i].first);
        strs.push_back(sub);
    }
    return strs;
}



int main(int argc, char* argv[]){
    if(argc < 2) {
        std::cout << "usage: "<< argv[0] <<" <lowest number> <highest number> <file>" << '\n';
        return 0;
    }
    
    std::string low;
    std::string hi;
    std::string file;
    std::string line;
    
    low.assign(argv[1]);
    hi.assign(argv[2]);
    file.assign(argv[3]);
    
    std::ifstream file_stream(file);
    
    if(file_stream.fail()){
        std::cout << "file could not be opened: " << file << '\n';
        std::exit(1);
    } else {
        std::cout << "file exists!" << '\n';
    }
    
    std::cout << "low: " << low << '\n';
    std::cout << "hi: " << hi << '\n';
    std::cout << "file: " << file << '\n';

    constexpr size_t buffersize = 4026 * 32;
    std::unique_ptr<char[]> buffer(new char[buffersize]);
    
    while( file_stream ) {
        file_stream.read(buffer.get(), buffersize);
        line.assign(buffer.get());
        std::vector<std::pair<int, int>> inds = find_numerics(line);
        if(inds.size() > 0){
            /*
            std::cout << "contains number(s)!" << '\n';
            for(int i = 0; i < inds.size(); i++){
                std::pair<int, int> p = inds[i];
                std::cout << "(" << p.first << ", " << p.second << ")"  << '\n';
            }
            */
            
            std::vector<std::string> subs = extract_substrings(inds, line);
            //std::cout << "subs.size(): " << subs.size() << '\n';
            for(int i = 0; i < subs.size(); i++){
                std::cout << subs[i] << '\n';
            }
            subs.clear();
        } else {
            std::cout << "does not contain numbers" << '\n';
        }
        inds.clear();
        file_stream.clear();
        std::string().swap(line);
        std::getline(file_stream, line);
        //std::cout << "line addr: " << &line << '\n';
        
    }
    
    return 0;
}





































