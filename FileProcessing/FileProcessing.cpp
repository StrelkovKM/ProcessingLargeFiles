
#include "FileProcessing.h"
#include <windows.h>

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