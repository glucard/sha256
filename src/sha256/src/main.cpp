#include "sha256.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream> //std::stringstream
#include <iostream>

#include <chrono>

std::string readFile(std::string file_name) {
    std::ifstream inFile;
    inFile.open(file_name); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    return str;
}

int main() {
    std::string str = "abc";
    std::cout << "           SHA256 coded by https://github.com/glucard .";
    while (str != "") {
        std::cout << "\n\n////////////////////////////////////////////////////////////////////////////////\n\nInput: ";
        str = "";
        getline(std::cin,str);
        auto start = std::chrono::high_resolution_clock::now();
        std::string digest = sh::sha256(str.c_str());
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "\nOutput(digest): " << digest;
        std::cout << " (" << (std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count()) * pow(10,-6) << "s)\n";
    }
	return 0;
}