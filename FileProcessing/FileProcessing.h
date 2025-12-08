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
    size_t size_of_RAM_byte;

    std::string input_filename;
    int input_descriptor; 
    size_t len_of_file_input;

    std::string output_filename;
    int output_descriptor;
    
    void SizeRAM();
    void LenInputFile();

    public:
    FileProcessing(const std::string& name_of_file_input_, const std::string& name_of_file_output_);

    ~FileProcessing();
    
    void SetSizeRAM(size_t size_of_ram);


    void CopyPartFileToRAM(size_t size_of_empty_RAM); 
    void EraseFromFile(size_t chunk_erase); //Миша
    void ShaffleInRAM(); //Егорка
    void WriteToFile(int file_descriptor); //Миша
};