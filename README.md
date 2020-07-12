# Range Grep :mag:
![GitHub](https://img.shields.io/github/license/alex-m159/range_grep)
![GitHub repo size](https://img.shields.io/github/repo-size/alex-m159/range_grep?style=plastic)
![GitHub All Releases](https://img.shields.io/github/downloads/alex-m159/range_grep/total?style=plastic)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/alex-m159/range_grep)

This is a command-line utility that lets you search within an ASCII-encoded text file for numbers that are within a given range.

## Why should you use this instead of grep?
Range Grep has a simpler interface. If you use grep you're going to have to specify the range you want to search for by providing a regular expression. With Range Grep you just have to specify the two numbers to search between and a file name, that's it.

## How does this compare to grep in terms of performance?
I tested grep and Range Grep on file sizes from 300 MB to 2.7 GB and Range Grep is ~2.5 seconds slower than grep in the worst case. (Note: If you use the `--color` argument then Range Grep becomes much much slower. These numbers are for output without that flag.)

## How To Use
Usage: `./range <lowest number> <highest number> <file>`

### Command Line Arguments:
`--color` Print out the matches in RED on the terminal. Defaults to no color.
        

## How To Build
Install the GCC C++ compiler and run the `build.sh` script. There should be no output at all if the compilation is successful. 

There's no build system or Make file. I intentionally limited myself to only the C and C++ standard library and a simple build script since this is such a small project.

## How To Run Tests
`./test.sh`

Expected output:
```bash
$ ./test.sh 
Compiling range.cpp
Compiling test_range.cpp
Linking
Running
```
If you get the output above, that means that all the tests passed.

