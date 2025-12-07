#include "FileProcessing.h"




FileProcessing::FileProcessing(const std::string& name_of_file_input_,
    const std::string& name_of_file_output_)
    : RAM(),
    infile(name_of_file_input_),
    outfile(name_of_file_output_)
{
    this->SizeRAM();
    this->LenFile();
    if (!infile.is_open() && !outfile.is_open())
        throw "error open files";
}

void FileProcessing::SetSizeRAM(size_t size_of_ram)
{
    size_of_RAM_byte = size_of_ram;
}

/*
void EraseFromFile(const std::string& name_of_file_input_, size_t size_to_erase) 
{
    HANDLE hFile = CreateFileA(name_of_file_input_.c_str(), GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Не удалось открыть файл для обрезки");
    }
    size_t new_size = len_of_file_input_ - size_to_erase;
    LARGE_INTEGER li;
    li.QuadPart = new_size;
    
    if (!SetFilePointerEx(hFile, li, NULL, FILE_BEGIN) || !SetEndOfFile(hFile)) {
        CloseHandle(hFile);
        throw std::runtime_error("Не удалось обрезать файл");
    }
    
    CloseHandle(hFile);
}
*/



// void FileProcessing::SizeRAM()
// {
//     MEMORYSTATUSEX memInfo;
//     memInfo.dwLength = sizeof(MEMORYSTATUSEX);

//     if (GlobalMemoryStatusEx(&memInfo)) {

//         std::cout << "Free RAM: " << memInfo.ullAvailPhys << " B\n";
//         size_of_RAM_byte = memInfo.ullAvailPhys;
//     }
//     else {
//         std::cout << "Error of reading of RAM size\n";
//         DWORD error = GetLastError();
//         std::cout << "Error code: " << error << std::endl;
//     }
// }


void FileProcessing::LenFile()
{
    std::streampos current_pos = infile.tellg();
    infile.seekg(0, std::ios::end);
    size_t size = infile.tellg();
    infile.seekg(current_pos);
    len_of_file_input = size;
    std::cout << "File size: " << size << " B\n";
}