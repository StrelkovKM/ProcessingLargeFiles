#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#include "FileProcessing.h"


void write_large_file(const std::string& filename)
{
    size_t i = 0;
    size_t count = 0;

    std::ofstream file(filename, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open file");

    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t target_size = static_cast<size_t>(32) * 1024 * 1024 * 1024;

    while (i < target_size)
    {
        std::string line = std::to_string(count);

        std::uniform_int_distribution<size_t> dis(1, 1024 - line.size());
        size_t zeros_to_add = dis(gen);
        line.append(zeros_to_add, '0');

        file.write(line.c_str(), line.size());

        if (!file)
            throw std::runtime_error("Failed to write to file");

        file.put('\n');
        if (!file)
            throw std::runtime_error("Failed to write newline to file");

        i += line.size() + 1;
        ++count;
    }

    file.close();
}

int main () {
    FileProcessing file_processor("/home/strelkovkm/ProcessingLargeFiles/src/InputFile.txt", "/home/strelkovkm/ProcessingLargeFiles/src/OutputFile.txt");
    file_processor.SetSizeRAM(15);
    file_processor.CopyPartFileToRAM(15);
    file_processor.EraseFromFile(8);
    file_processor.ShaffleInRAM();
    file_processor.CopyPartFileToRAM(15);
    file_processor.EraseFromFile(13);
    file_processor.ShaffleInRAM();
}
