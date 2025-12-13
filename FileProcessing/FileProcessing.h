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

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <unordered_map>
#endif

class FileProcessing {
    private:
    std::fstream file;
    std::vector<char> buffer;//������ ������ ��������� ������, ������� ����� ���� ��������� �� �� �����
    std::vector<char> slice;//������ ����� ������ ����� ���������� ��� ��������� �� ������
    std::vector<std::vector<char>> memory;//������ ������ ����� ������ �� ��������� ����� ������
    size_t memory_size;//���������� ������ �� �����, ������� �������� � memory
    size_t ram_size;//���������� ����������� ������, ������� ��� ��������
    size_t start_write;//�������, �� ������� ���������� ������������ ������ � ����
    size_t start_read;//�������, � ������� ��������� ����� ������ �� �����

    size_t size_of_file;//����� ������ � �����/���������� �������� � �����

    public:
    FileProcessing(const std::string& filename);
    ~FileProcessing();

    void sizeRAM();
    void setRAMSize(size_t size);
    void lenFile();
    void printRAM();
    void readFile(); //Egor
    void spliteMemory(); //Misha
    void clearSlice();
    void clearBuffer();
    void shuffleMemory(); //Egor
    void mergeSlice();
    void writeFile(); //Misha

    void executeProcessing();
};