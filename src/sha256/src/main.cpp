#include "sha256.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream> //std::stringstream
#include <iostream>

std::string readFile(std::string file_name) {
    std::ifstream inFile;
    inFile.open(file_name); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    return str;
}

int main() {
	//bn::BinaryNumber* b = sh::createBinaryMessage("abc");
	//bn::promptData(b);
	//bn::destroyBinaryNumber(b);

    std::string digest = sh::sha256("");
    std::cout << digest;
	return 0;
}