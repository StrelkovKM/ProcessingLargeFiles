#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <random>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <unordered_map>
#endif

class FileProcessing {
    private:
    std::fstream file;//Объект файла с данными
    std::vector<char> buffer;//Хранит начало последней считанной строки, которая может быть считана не до конца
    std::vector<char> slice;//Хранит сырые данные после считывания без разбиения на строки
    std::vector<std::vector<char>> memory;//Хранит только целые строки из считанных сырых данных
    size_t memory_size;//Количество данных из файла, которые хранятся в memory
    size_t ram_size;//Количество оперативной памяти, доступной для работы программы
    size_t start_write;//Позиция в файле, с которой записываются перемешанные строки
    size_t start_read;//Позиция в файле, с которой считываются сырые данные
    size_t size_of_file;//Количество символов в файле

    //Кирилл Стрелков
    void sizeRAM();//Вспомогательный метод, передает количество свободной RAM полю ram_size
    void lenFile();//Вспомогательный метод, передает количество символов в файле полю size_of_file
    void mergeSlice();//Вспомогательный метод, объединяющий строку, считанную не полностью из поля buffer, с новыми сырыми данными

    void logRAM();//Метод для отладки

    public:
    FileProcessing(const std::string& filename);//Конструктор от имени файла
    ~FileProcessing();//Деструктор

    //Кирилл Стрелков
    void executeProcessing();//Метод для запуска работы программы
    void setRAMSize(size_t size);//Метод для указания количества RAM, доступной программе

    //Егор Турушкин
    void readFile();//Метод для считывания сырых данных в slice
    void shuffleMemory();//Метод для перемешивания строк поля memory (Реализован алгоритм Фишера-Йетса)

    //Михаил Порфирьев
    void writeFile(); //Метод, записывающий перемешанные строки в файл
    void spliteMemory(); //Метод, делящий сырые данные на строки
};
