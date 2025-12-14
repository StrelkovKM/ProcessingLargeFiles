#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "FileProcessing.h"

int main() 
{
    auto start = std::chrono::high_resolution_clock::now();
    FileProcessing A("/home/strelkovkm/ProcessingLargeFiles/src/InputPut.txt");
    A.setRAMSize(25);
    A.executeProcessing(); 
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Time: " << duration.count() / 1000 << " s\n";
}