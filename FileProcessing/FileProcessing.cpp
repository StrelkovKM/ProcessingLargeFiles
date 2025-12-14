#include "FileProcessing.h"


FileProcessing::FileProcessing(const std::string &filename) : 
    memory_size(0), ram_size(0), start_write(0), start_read(0)
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

void FileProcessing::printRAM()
{
    std::cout << "[INFO] SLICE: " << slice.size() << "\n";
    for (const auto& it : slice) {
        std::cout << it;
    }
    std::cout << "\n";

    std::cout << "[INFO] MEMORY: " << memory_size << " [" << memory.size() << "]" << "\n";
    for (const auto& it1 : memory) {
        for (const auto& it2 : it1) {
            std::cout << it2;
        }
    }
    std::cout << "\n";

    std::cout << "[INFO] BUFFER: " << buffer.size() << "\n";
    for (const auto& it : buffer) {
        std::cout << it;
    }
    std::cout << "\n";
}

void FileProcessing::readFile()
{
    std::cout << "========================START_READ=========================\n";
    size_t empty_space = (ram_size - memory_size - buffer.size()) / 2;
    size_t chunk_read = ( (start_read + empty_space) > size_of_file)
        ? (size_of_file - start_read) : ( empty_space );
    std::cout << "[INFO] START_READ: " << start_read << "\n";
    std::cout << "[INFO] CHUNK_READ: " << chunk_read << "\n";
    slice.resize(chunk_read);
    file.seekg(start_read);
    file.read(slice.data(), chunk_read);
    start_read += chunk_read;
    //printRAM();
    std::cout << "========================STOP_READ=========================\n";
}

void FileProcessing::spliteMemory()
{
    std::cout << "========================START_SPLITE=========================\n";

    auto it = slice.begin();
    if (!memory.empty() && memory.back().back() != '\n') {
        it = std::find(it, slice.end(), '\n');
        memory.back().insert(memory.back().end(), slice.begin(), it + 1);
        ++it;
    }
    auto start = it;
    while ((it = std::find(it, slice.end(), '\n')) != slice.end())
    {
        memory.push_back(std::vector<char>(start, it + 1));
        ++it;
        start = it;
    }
    if (start != slice.end())
        buffer.assign(start, slice.end());

    memory_size += slice.size() - buffer.size();
    //printRAM();
    std::cout << "========================STOP_SPLITE=========================\n";
}

void FileProcessing::clearSlice()
{
    std::cout << "=====================START_CLEAR_SLICE======================\n";
    if (!slice.empty()) {
        slice.clear();
    }
    //printRAM();
    std::cout << "=====================STOP_CLEAR_SLICE=======================\n";
}

void FileProcessing::clearBuffer()
{
    std::cout << "====================START_CLEAR_BUFFER=====================\n";
    if (!buffer.empty()) {
        buffer.clear();
    }
    //printRAM();
    std::cout << "====================STOP_CLEAR_SLICE======================\n";
}

void FileProcessing::shuffleMemory()
{
    std::cout << "========================START_SHUFFLE=========================\n";
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

    //printRAM();
    std::cout << "========================END_SHUFFLE=========================\n";
}

void FileProcessing::mergeSlice()
{
    std::cout << "========================START_MERGE=========================\n";

    size_t start_index = (start_read != size_of_file) ? memory.size() / 2 : 0;
    auto start_it = memory.begin() + start_index;

    for (auto it = start_it; it != memory.end(); ++it) {
        slice.insert(slice.end(), std::make_move_iterator(it->begin()), std::make_move_iterator(it->end()));
        memory_size -= it->size(); 
    }
    
    memory.erase(start_it, memory.end());


    if(!buffer.empty()) {
        memory.push_back(buffer);
        memory_size += buffer.size();
    }
    
    //printRAM();
    std::cout << "========================STOP_MERGE=========================\n";
}

void FileProcessing::writeFile()
{
    std::cout << "========================START_WRITE=========================\n";
    file.seekp(start_write, std::ios::beg);
    file.write(slice.data(), slice.size());
    std::cout << "[INFO] WRITE_POS: " << start_write << "\n";
    start_write = file.tellp();
    //printRAM();
    std::cout << "========================STOP_WRITE=========================\n";
}

void FileProcessing::executeProcessing()
{
    size_t n = 0;
    while (size_of_file != start_read) {
        readFile();
        spliteMemory();
        clearSlice();
        shuffleMemory();
        mergeSlice();
        clearBuffer();
        writeFile();
        clearSlice();
        n++;
    }
    std::cout << "\n[INFO] TOTAL_ITERATIONS: " << n << "\n";
    memory.clear();
}
