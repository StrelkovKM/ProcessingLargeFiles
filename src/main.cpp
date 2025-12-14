#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "FileProcessing.h"

int main() 
{
    auto start = std::chrono::high_resolution_clock::now();

    FileProcessing A("<path_file>");
    //Example: A.setRAMSize(0.25 * 1024 * 1024 * 1024);// 0.25 GB
    A.executeProcessing(); 

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time: " << duration.count() / 1000 << " s\n";
    return 0;
}