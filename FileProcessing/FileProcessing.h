#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>


class FileProcessing {
    private:
    std::vector<std::string> RAM;
    size_t size_of_RAM_byte;
    size_t len_of_file_input;
    HANDLE infile;
    HANDLE outfile;
    size_t chunk;
    size_t chunk_erase;
    
    void SizeRAM(); //Егорка
    void LenFile(); //Миша


    public:
    FileProcessing(const std::string& name_of_file_input_, const std::string& name_of_file_output_);
    void SetSizeRAM(size_t size_of_ram);


    void CopyPartFileToRAM(); 
    void EraseFromFile(); //Миша
    void ShaffleInRAM(); //Егорка
    void WriteToFile(std::string name_of_file_output_);
};