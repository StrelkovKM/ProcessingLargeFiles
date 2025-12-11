#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <unordered_map>
#endif

class FileProcessing { //Kirill
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

    size_t size_of_file;

    public:
    FileProcessing(const std::string& filename); //Kirill
    ~FileProcessing(); //Kirill

    void sizeRAM(); //Kirill
    void setRAMSize(size_t size); //Kirill
    void lenFile(); //Kirill

    void readFile(); //Egor
    void spliteMemory(); //Misha
    void clearSlice(); //Kirill
    void shuffleMemory(); //Egor
    void mergeSlice(); //Kirill
    void writeFile(); //Misha

    void executeProcessing(); //Kirill
};