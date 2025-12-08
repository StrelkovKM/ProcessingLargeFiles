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
    FileProcessing file("/home/strelkovkm/ProcessingLargeFiles/src/InputFile.txt",
        "/home/strelkovkm/ProcessingLargeFiles/src/OutputFile.txt");
    file.SetSizeRAM(4 * 1024);
}

