#include <iostream>
#include <random>
#include <fstream>
#include <chrono>
#include <string>

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
    // auto start = std::chrono::high_resolution_clock::now();
    // FileProcessing file_processor("/home/strelkovkm/ProcessingLargeFiles/src/TEST_32GB.txt", "/home/strelkovkm/ProcessingLargeFiles/src/OutputFile.txt");
    // file_processor.SetSizeRAM(0.25 * 1024 * 1024 * 1024);
    // file_processor.ExecuteProcessing();
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // std::cout << "Time: " << duration.count() / 1000 << " s\n";

    std::vector<std::vector<char>> memory { {}, {} };
    std::fstream file_in ("/home/strelkovkm/ProcessingLargeFiles/src/InputFile.txt", std::ios::in | std::ios::out);
    size_t free_memory = 18;
    size_t start_write = 0;
    size_t start_read = 0;
    size_t chunk_rewrite = 0;
    std::vector<char> buffer;
    memory[0].resize(free_memory);
    
    file_in.seekg(0, std::ios::beg);
    file_in.read(memory[0].data(), free_memory);
    for (int i = 0; i < free_memory; ++i) {
        std::cout << memory[0][i];
        buffer.push_back(memory[0][i]);
    }
    for (int i = free_memory, it = 1; i < free_memory; i++) {
        std::cout << memory[0][i];
        memory[it].push_back(memory[0][i]);
        if (memory[0][i] == '\n' && i != free_memory - 1) {
            memory.push_back(std::vector<char>());
            ++it;
        }
    }
    std::cout << "\n===========BUFFER===========\n";
    for (int i = 0; i < buffer.size(); i++) {
        std::cout << buffer[i];
    }
    std::cout << "============MEM============\n";
    for (int j = 1; j < memory.size(); ++j) {
        std::cout << "Line " << j << ": ";
        for (int k = 0; k < memory[j].size(); ++k) {
            std::cout << memory[j][k];
        }
    }

    file_in.close();
}
