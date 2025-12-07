#include <iostream>
#include <vector>
#include <string>
#include <fstream>


class FileProcessing {
    private:
    std::vector<std::string> RAM;
    size_t size_of_RAM_byte;
    std::string name_of_file_output;
    size_t len_of_file_input;
    std::string name_of_file_input;
    
    //size_t size_of_RAM_Mbyte = size_of_RAM_byte / ( 1024 * 1024);

    void SizeRAM(); //Егорка
    void LenFile(std::string name_of_file); //Миша

    public:
    FileProcessing(std::string name_of_file_input_);

    void CopyPartFileToRAM(size_t size_of_empty_RAM); 
    void EraseFromFile(std::string name_of_file_input_, size_t size_to_erase); //Миша
    void ShaffleInRAM(); //Егорка
    void WriteToFile(std::string name_of_file_output_);
};