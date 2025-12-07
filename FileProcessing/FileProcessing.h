#include <iostream>
#include <vector>
#include <string>
#include <fstream>


class FileProcessing {
    private:
    std::vector<std::string> RAM;
    size_t size_of_RAM_byte;
    size_t len_of_file_input;
    std::fstream infile;
    std::fstream outfile;
    
    void SizeRAM(); //Егорка
    void LenFile(); //Миша


    public:
    FileProcessing(const std::string& name_of_file_input_, const std::string& name_of_file_output_);
    void SetSizeRAM(size_t size_of_ram);


    void CopyPartFileToRAM(size_t size_of_empty_RAM); 
    void EraseFromFile(std::string name_of_file_input_, size_t size_to_erase); //Миша
    void ShaffleInRAM(); //Егорка
    void WriteToFile(std::string name_of_file_output_);
};