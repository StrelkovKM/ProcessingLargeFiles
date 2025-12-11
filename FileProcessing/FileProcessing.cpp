#include "FileProcessing.h"

FileProcessing::FileProcessing(const std::string &filename) : 
    memory_size(0), ram_size(0), start_write(0), start_read(0), chunk_read(0)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

FileProcessing::~FileProcessing()
{
    file.close();
}

void FileProcessing::spliteMemory()
{
    auto start = slice.begin();
    auto it = slice.begin();
    while ((it = std::find(it, slice.end(), '\n')) != slice.end())
    {
        memory.push_back(std::vector<char>(start, it + 1));
        ++it;
        start = it;
    }
    if (start != slice.end())
        buffer.assign(start, slice.end());
}

void FileProcessing::clearSlice()
{
    if (!memory[0].empty()) {
        memory.clear();
    }
}
