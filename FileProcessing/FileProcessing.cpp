#include "FileProcessing.h"

FileProcessing::FileProcessing(const std::string& input_filename_, const std::string& output_filename_)
    : RAM(), input_descriptor(-1), output_descriptor(-1), size_of_RAM(0), chunk_erase(0), memory_at_RAM(0)
    , input_filename(input_filename_), output_filename(output_filename_)
{
    input_descriptor = open(input_filename.c_str(), O_RDWR);
    if (input_descriptor == -1) {
        std::string error = "The input file could not be opened '" + input_filename + "': " + strerror(errno);
        throw std::runtime_error(error);
    }

    output_descriptor = open(output_filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (output_descriptor == -1) {
        close(input_descriptor);
        std::string error = "The input file could not be opened'" + output_filename + "': " + strerror(errno);
        throw std::runtime_error(error);
    }

    try {
        SizeRAM();
    } catch (const std::exception& e) {
        close(input_descriptor);
        close(output_descriptor);
        throw std::runtime_error("RAM detection error: " + std::string(e.what()));
    }

    try {
        LenInputFile();
    } catch (const std::exception& e) {
        close(input_descriptor);
        close(output_descriptor);
        throw std::runtime_error("File size detection error: " + std::string(e.what()));
    }

    std::cout << "The input file: " << input_filename << " (" << len_input_file << " Byte)" << "\n";
    std::cout << "The output file: " << output_filename << "\n";
    std::cout << "RAM available: " << size_of_RAM << "\n";
}

FileProcessing::~FileProcessing()
{
    if (input_descriptor != -1) close(input_descriptor);
    if (output_descriptor != -1) close(output_descriptor);
}

void FileProcessing::SetSizeRAM(size_t size_of_RAM_)
{
    size_of_RAM = size_of_RAM_;
}


void FileProcessing::SizeRAM()
{
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
    size_of_RAM = static_cast<size_t>( (it->second * 1024) * 0.7 );
}


void FileProcessing::LenInputFile() {
    struct stat st;
    if (fstat(input_descriptor, &st) != 0) {
        throw std::runtime_error("Couldn't get file size: ");
    }
    len_input_file = st.st_size;
}

void FileProcessing::ClearHalfRAM()
{
    std::cout << "========================START_CLEAR=========================\n";
    if (RAM.empty()) {
        std::cout << "[INFO] RAM is already empty\n";
        return;
    }

    size_t half = RAM.size() / 2;
    
    memory_at_RAM = 0;
    for (size_t i = 0; i < half; ++i) {
        memory_at_RAM += RAM[i].size() + 1;
    }
    
    RAM.erase(RAM.begin() + half, RAM.end());
    RAM.shrink_to_fit();
    std::cout << "[INFO] RAM: " << RAM.size() << " lines\n";
    for (const auto& line : RAM) {
        std::cout << line << "\n";
    }
    std::cout << "========================END_CLEAR=========================\n";
}

void FileProcessing::EraseFromFile() {
    std::cout << "========================START_ERASE=========================\n";
    if (len_input_file == 0) {
        std::cout << "[INFO] The file is already empty\n";
        return;
    }

    off_t new_size = len_input_file - chunk_erase;

    if (ftruncate(input_descriptor, new_size) == -1) {
        throw std::runtime_error("File Truncation error: " + std::string(strerror(errno)));
    }

    len_input_file = new_size;

    lseek(input_descriptor, 0, SEEK_END);

    std::cout << "[INFO] Deleted: " << chunk_erase << " byte\n";
    std::cout << "[INFO] New file size: " << len_input_file << " byte\n";
    std::cout << "========================END_ERASE=========================\n";
}

void FileProcessing::ShaffleInRAM()
{
    std::cout << "========================START_SHAFFLE=========================\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    
    size_t n = RAM.size();
    
    if (n < 2) return;
    
    for (size_t i = 0; i < n - 1; ++i)
    {
        std::uniform_int_distribution<size_t> dis(i, n - 1);
        size_t j = dis(gen);
        
        std::swap(RAM[i], RAM[j]);
    }

    std::cout << "[INFO] RAM: " << RAM.size() << " lines\n";
    for (const auto& line : RAM) {
        std::cout << line << "\n";
    }
    std::cout << "========================END_SHAFFLE=========================\n";
}

void FileProcessing::WriteToFile()
{
    std::cout << "========================START_WRITE=========================\n";
    size_t shift =  0; //(len_input_file > chunk_erase) ? (RAM.size() / 2 + RAM.size() % 2) : 0;
    if ((len_input_file > chunk_erase) && RAM.size() != 1) {
        shift = RAM.size() / 2 + RAM.size() % 2;
    }
    if (lseek(output_descriptor, 0, SEEK_END) == -1) {
        throw std::runtime_error("Positioning error in the output file");
    }
    
    for (auto it = RAM.begin() + shift; it != RAM.end() ; ++it) {
        ssize_t bytes_written = write(output_descriptor, (*it).c_str(), (*it).size());
        if (bytes_written == -1) {
            throw std::runtime_error("Write error to output file: " + std::string(strerror(errno)));
        }
        std::cout << (*it) << '\n';
    }
    fsync(output_descriptor);
    std::cout << "========================END_WRITE=========================\n";
}

void FileProcessing::CopyPartFileToRAM()
{
    std::cout << "========================START_COPY=========================\n";
    size_t start_pos = (len_input_file  > (size_of_RAM - memory_at_RAM)) ? (len_input_file - (size_of_RAM - memory_at_RAM) - 1) : 0;
    std::cout << "start_pos: " << start_pos << '\n';
    std::cout << "len_of_file: " << len_input_file << '\n'; 
    char symbol = '\0';

    //optimaze //добавить прикол с предыдущим
    lseek(input_descriptor, start_pos, SEEK_SET);
    ssize_t n = read(input_descriptor, &symbol, 1);
    while(symbol != '\n' && n != -1) {
        lseek(input_descriptor, ++start_pos, SEEK_SET);
        n = read(input_descriptor, &symbol, 1);
    }

    chunk_erase = len_input_file - start_pos - 1;
    std::cout << "[INFO] Chunk to erase: " << chunk_erase << " byte\n";

    //optimaze
    ++start_pos;
    std::cout << "start_pos: " << start_pos << '\n';
    std::string buffer;
    while (start_pos != len_input_file && n != -1) {
        lseek(input_descriptor, start_pos, SEEK_SET);
        ssize_t n = read(input_descriptor, &symbol, 1);
        buffer.push_back(symbol);
        if(symbol == 10)
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
    memory_at_RAM += chunk_erase;

    std::cout << "========================END_COPY=========================\n";
}


void FileProcessing::ExecuteProcessing()
{
    // Первая итерация
    if (len_input_file == 0) {
        std::cout << "[INFO] Input file is empty\n";
        return;
    }

    CopyPartFileToRAM();
    ShaffleInRAM();
    EraseFromFile();
    WriteToFile();
    ClearHalfRAM();

    while (len_input_file > 0 && len_input_file > (size_of_RAM - memory_at_RAM)) {
        CopyPartFileToRAM();
        if (RAM.empty()) break;
        
        ShaffleInRAM();
        WriteToFile();
        EraseFromFile();

        ClearHalfRAM();
    }

    // Последний кусок
    if (len_input_file > 0) {
        CopyPartFileToRAM();
        if (!RAM.empty()) {
            ShaffleInRAM();
            WriteToFile();
            EraseFromFile();
        }
    }

    // Очищаем RAM
    RAM.clear();
    memory_at_RAM = 0;
    chunk_erase = 0;
    
    std::cout << "[INFO] Processing completed\n";
}
