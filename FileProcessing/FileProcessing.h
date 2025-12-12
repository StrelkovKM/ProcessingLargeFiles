#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <algorithm>
#include <random>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <unordered_map>
#endif

class FileProcessing {
    private:
    std::fstream file;
    std::vector<char> buffer;//Хранит начало последней строки, которая может быть считатана не до конца
    std::vector<char> slice;//Хранит сырые данные после считывания без разбиения на строки
    std::vector<std::vector<char>> memory;//Хранит только целые строки из считанных сырых данных
    size_t memory_size;//Количество данных из файла, которые хранятся в memory
    size_t ram_size;//Количество оперативной памяти, которая нам доступна
    size_t start_write;//Позиция, на которую записываем перемешанные строки в файл
    size_t start_read;//Позиция, с которой считываем сырые данные из файла
    size_t chunk_read;

    size_t size_of_file;//Обьем данных в файле/Количество символов в файле

    public:
    FileProcessing(const std::string& filename);
    ~FileProcessing();

    void sizeRAM();
    void setRAMSize(size_t size);
    void lenFile();

    void readFile(); //Egor
    void spliteMemory(); //Misha
    void clearSlice();
    void shuffleMemory(); //Egor
    void mergeSlice();
    void writeFile(); //Misha

    void executeProcessing();
};