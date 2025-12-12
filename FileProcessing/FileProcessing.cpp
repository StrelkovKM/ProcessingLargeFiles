#include "FileProcessing.h"


FileProcessing::FileProcessing(const std::string &filename) : 
    memory_size(0), ram_size(0), start_write(0), start_read(0), chunk_read(0)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    lenFile();
    sizeRAM();
}

FileProcessing::~FileProcessing()
{
    if (file.is_open()) {
        file.close();
    }
}

void FileProcessing::sizeRAM()
{
    #ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&memInfo)) {

            std::cout << "Free RAM: " << memInfo.ullAvailPhys << " B\n";
            ram_size = static_cast<size_t>(memInfo.ullAvailPhys*0.7);
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

void FileProcessing::setRAMSize(size_t size)
{
    ram_size = size;
    slice.clear();
}

void FileProcessing::lenFile()
{
    file.seekg(0, std::ios::end);
    size_of_file = file.tellg();
    file.seekg(0, std::ios::beg);
}

void FileProcessing::readFile()
{
    size_t current_read = (start_read + (ram_size - memory_size) > size_of_file)
        ? (size_of_file - start_read)
        : (ram_size - memory_size);
    slice.resize(current_read);
    file.seekg(start_read);
    file.read(slice.data(), current_read);
    start_read += current_read;
    std::cout << "\n------------------------------" << slice.size()<<'\n';
    for (size_t i = 0; i < slice.size(); ++i)
        std::cout << slice[i];
}

void FileProcessing::spliteMemory()
{
    auto start = slice.begin();
    auto it = slice.begin();
    while ((it = std::find(it, slice.end(), '\n')) != slice.end())
    {
        memory.push_back(std::vector<char>(start, it + 1));
        ++it;
        start = it;
    }
    if (start != slice.end())
        buffer.assign(start, slice.end());
}

void FileProcessing::clearSlice()
{
    if (!slice.empty()) {
        memory_size -= slice.size();
        slice.clear();
    }
}

void FileProcessing::shuffleMemory()
{
    std::cout << "========================START_SHAFFLE=========================\n";
    std::random_device rd;
    std::mt19937 gen(rd());

    size_t n = memory.size();

    if (n < 2) return;

    for (size_t i = 0; i < n - 1; ++i)
    {
        std::uniform_int_distribution<size_t> dis(i, n - 1);
        size_t j = dis(gen);

        std::swap(memory[i], memory[j]);
    }

    std::cout << "[INFO] RAM: " << memory.size() << " lines\n";
    std::cout << "========================END_SHAFFLE=========================\n";
}

void FileProcessing::mergeSlice()
{
    for (auto it = memory.begin() + memory.size() / 2; it != memory.end(); ++it) {
        memory_size += it->size();
        slice.insert(slice.end(), it->begin(), it->end());
    }
}

void FileProcessing::executeProcessing()
{

}
