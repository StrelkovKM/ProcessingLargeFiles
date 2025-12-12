#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileProcessing.h"

int main() 
{
    FileProcessing a("InputFile.txt");
    a.setRAMSize(5);
    a.readFile();
    a.readFile();
}