#include "FileProcessing.h"


FileProcessing::FileProcessing(const std::string &filename) : 
    memory_size(0), ram_size(0), start_write(0), start_read(0), chunk_read(0)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

FileProcessing::~FileProcessing()
{
    file.close();
}

void FileProcessing::sizeRAM()
{
    #ifdef _WIN32
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
    #elif __linux__
            std::ifstream mem_info("/proc/meminfo");
        if (!mem_info.is_open()) {
            throw std::runtime_error("Couldn't open /proc/meminfo");
        }

        std::unordered_map<std::string, size_t> mem_data;
        std::string line;

        while (std::getline(mem_info, line)) {
            std::istringstream iss(line);
            std::string key;
            size_t value;
            std::string unit;

            iss >> key;
            if (key.back() == ':') {
                key.pop_back();
            }

            iss >> value >> unit;
            mem_data[key] = value;
        }

        auto it = mem_data.find("MemAvailable");
        ram_size = static_cast<size_t>( (it->second * 1024) * 0.7 );
    #endif
}   

void     FileProcessing::setRAMSize(size_t size)
{
    ram_size = size;
}


void FileProcessing::clearSlice()
{
    if (!slice.empty()) {
        slice.clear();
    }
}

void FileProcessing::mergeSlice()
{
    for (auto it = memory.begin() + memory.size() / 2; it != memory.end(); ++it) {
        slice.insert(slice.end(), it->begin(), it->end());
    }
}
