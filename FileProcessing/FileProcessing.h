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


class FileProcessing {
    private:
    std::vector<std::string> RAM;
    size_t size_of_RAM;

    std::string input_filename; //28+len(input_filename)*sizeof(char)
    int input_descriptor;  //sizeof(int)
    size_t len_input_file; //sizeof(size_t)

    std::string output_filename; //28+len(input_filename)*sizeof(char)
    int output_descriptor; //sizeof(int)
    
    void SizeRAM();
    void LenInputFile();

    public:
    FileProcessing(const std::string& input_filename_, const std::string& output_filename_);

    ~FileProcessing();
    
    void SetSizeRAM(size_t size_of_RAM_);

    void CopyPartFileToRAM(size_t size_of_empty_RAM); 
    void EraseFromFile(size_t chunk_erase);
    void ShaffleInRAM(); //Егорка
    void WriteToFile(int file_descriptor);
};