#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#include "FileProcessing.h"

size_t random_len(size_t max_len) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(1, max_len);
    return dis(gen);
}

int main () {

    //std::ofstream inputFile("InputFile.txt");
    //if (!inputFile.is_open()) {
    //    std::cerr << "Error opening file!" << std::endl;
    //    return 1;
    //}

    //if (inputFile.is_open()) {
    //    size_t len_of_row = 1000;
    //    for (size_t i = 0; i < len_of_row; i++) {
    //        size_t len_of_str = random_len(1020);
    //        inputFile << i;
    //        for (size_t j = 0; j < len_of_str; j++) {
    //            inputFile << 0;
    //        }
    //        inputFile << "\n";
    //    }
    //}

    FileProcessing file("InputFile.txt","OutputFile.txt");
    file.SetSizeRAM(4 * 1024);
}

