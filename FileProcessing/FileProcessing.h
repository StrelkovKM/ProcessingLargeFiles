#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdexcept>
#include <random>


class FileProcessing {
    private:
    std::vector<std::string> RAM;
    size_t size_of_RAM;

    std::string input_filename;
    int input_descriptor;
    size_t len_input_file;

    std::string output_filename;
    int output_descriptor;
    
    size_t chunk_erase;
    
    void SizeRAM();
    void LenInputFile();
    void ClearHalfRAM(); 

    public:
    FileProcessing(const std::string& input_filename_, const std::string& output_filename_);

    ~FileProcessing();
    
    void SetSizeRAM(size_t size_of_RAM_);
    void CopyPartFileToRAM(size_t size_of_empty_RAM); 
    void EraseFromFile(size_t chunk_erase_);
    void ShaffleInRAM();
    void WriteToFile(int file_descriptor);
};