#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

class FileProcessing {
    private:
    std::fstream file;
    std::vector<char> buffer;
    std::vector<char> slice;
    std::vector<std::vector<char>> memory;
    size_t memory_size;
    size_t ram_size;
    size_t start_write;
    size_t start_read;
    size_t chunk_read;

    public:
    FileProcessing(const std::string& filename);
    ~FileProcessing();

    void sizeRAM();
    void setRAMSize(size_t size);

    void readFile(); //Egor
    void spliteMemory(); //Misha
    void clearSlice();
    void shuffleMemory(); //Egor
    void mergeSlice();
    void writeFile(); //Misha

    void executeProcessing();
};