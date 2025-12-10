#include "FileProcessing.h"
#include "Windows.h"
#include <algorithm>




FileProcessing::FileProcessing(const std::string& name_of_file_input_,
    const std::string& name_of_file_output_)
    : RAM()
{
    infile = CreateFile(name_of_file_input_.c_str(),
        GENERIC_READ | GENERIC_WRITE | DELETE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL
    );

    if (infile == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Cannot open file. Error: " +
            std::to_string(GetLastError()));
    }

    outfile = CreateFile(name_of_file_output_.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL
    );

    if (outfile == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Cannot open file. Error: " +
            std::to_string(GetLastError()));
    }
    
    chunk = 0;
    this->SizeRAM();
    this->LenFile();

}

void FileProcessing::SetSizeRAM(size_t size_of_ram)
{
    size_of_RAM_byte = size_of_ram;
}

void FileProcessing::CopyPartFileToRAM()
{
    size_t start_pos = (len_of_file_input - (size_of_RAM_byte - chunk) > 0)
        ? (len_of_file_input - (size_of_RAM_byte - chunk)) : 0;
    char symbol = '\0';

    LARGE_INTEGER li;
    li.QuadPart = static_cast<LONGLONG>(start_pos);
    SetFilePointerEx(infile, li, NULL, FILE_BEGIN);
    
    DWORD n;

    while (symbol!=10)
    {
        ReadFile(infile, &symbol, 1, &n, NULL);
        start_pos++;
    }

    chunk_erase = len_of_file_input - start_pos - 1;
    std::cout << "[INFO] Chunk to erase: " << chunk_erase << " byte\n";

    std::string buffer;
    while (start_pos != len_of_file_input) {
        ReadFile(infile, &symbol, 1, &n, NULL);
        buffer.push_back(symbol);
        if (symbol == 10)
        {
            RAM.push_back(buffer);
            buffer.clear();
        }
        start_pos++;
    }
    std::cout << "[INFO] Copied to RAM: " << RAM.size() << " lines\n";
    for (const auto& line : RAM) {
        std::cout << line << "\n";
    }
}

void FileProcessing::EraseFromFile()
{
    if (len_of_file_input == 0) {
        std::cout << "[INFO] The file is already empty\n";
        return;
    }

    size_t actual_erase = (((chunk_erase) < (len_of_file_input)) ? (chunk_erase) : (len_of_file_input));
    LARGE_INTEGER li;
    li.QuadPart = static_cast<LONGLONG>(actual_erase);
    if (!SetFilePointerEx(infile, li , NULL, FILE_BEGIN))
        throw("1");

    // Обрезаем файл до этой позиции
    if (!SetEndOfFile(infile))
        throw("2");

}

void FileProcessing::SizeRAM()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {

        std::cout << "Free RAM: " << memInfo.ullAvailPhys << " B\n";
        size_of_RAM_byte = static_cast<size_t>(memInfo.ullAvailPhys*0.7);
    }
    else {
        std::cout << "Error of reading of RAM size\n";
        DWORD error = GetLastError();
        std::cout << "Error code: " << error << std::endl;
    }
}


void FileProcessing::LenFile()
{
    LARGE_INTEGER size_of_file;
    if (!GetFileSizeEx(infile,&size_of_file)) {
        CloseHandle(infile);
        throw std::runtime_error("Cannot get file size");
    }

    if (size_of_file.QuadPart < 0) {
        throw std::runtime_error("File size cannot be negative");
    }
    len_of_file_input = static_cast<size_t>(size_of_file.QuadPart);
}