#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileProcessing.h"

int main() 
{
    FileProcessing A("/home/strelkovkm/ProcessingLargeFiles/src/InputFile.txt");
    A.setRAMSize(20);

    std::cout << "========================FIRST_PASS=========================\n";
    A.readFile();
    A.spliteMemory();
    A.clearSlice();
    A.shuffleMemory();
    A.mergeSlice();
    A.clearBuffer();
    A.writeFile();
    A.clearSlice();

    std::cout << "========================SECOND_PASS=========================\n";

    A.readFile();
    A.spliteMemory();
    A.clearSlice();
    A.shuffleMemory();
    A.mergeSlice();
    A.clearBuffer();
    A.writeFile();
    A.clearSlice();

    std::cout << "========================THIRD_PROCESSING=========================\n";

    A.readFile();
    A.spliteMemory();
    A.clearSlice();
    A.shuffleMemory();
    A.mergeSlice();
    A.clearBuffer();
    A.writeFile();
    A.clearSlice();
}