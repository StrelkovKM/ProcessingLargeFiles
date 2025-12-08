#include "FileProcessing.h"

FileProcessing::FileProcessing(const std::string& name_of_file_input_, const std::string& name_of_file_output_)
    : RAM(), input_descriptor(-1), output_descriptor(-1), size_of_RAM_byte(0)
    , input_filename(name_of_file_input_), output_filename(name_of_file_output_)
{
    input_descriptor = open(input_filename.c_str(), O_RDWR);
    if (input_descriptor == -1) {
        std::string error = "Не удалось открыть входной файл '" + input_filename + "': " + strerror(errno);
        throw std::runtime_error(error);
    }

    output_descriptor = open(output_filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (output_descriptor == -1) {
        close(input_descriptor);
        std::string error = "Не удалось открыть выходной файл '" + output_filename + "': " + strerror(errno);
        throw std::runtime_error(error);
    }

    try {
        SizeRAM();
    } catch (const std::exception& e) {
        close(input_descriptor);
        close(output_descriptor);
        throw std::runtime_error("Ошибка определения RAM: " + std::string(e.what()));
    }

    try {
        LenInputFile();
    } catch (const std::exception& e) {
        close(input_descriptor);
        close(output_descriptor);
        throw std::runtime_error("Ошибка определения размера файла: " + std::string(e.what()));
    }

    std::cout << "FileProcessing инициализирован:\n";
    std::cout << "  Входной файл: " << input_filename << " (" << len_of_file_input << " байт)\n";
    std::cout << "  Выходной файл: " << output_filename << "\n";
    std::cout << "  Доступно RAM: " << size_of_RAM_byte << " байт (" 
              << size_of_RAM_byte / (1024*1024) << " MB)\n";
}

FileProcessing::~FileProcessing()
{
    if (input_descriptor != -1) close(input_descriptor);
    if (output_descriptor != -1) close(output_descriptor);
}

void FileProcessing::SetSizeRAM(size_t size_of_ram)
{
    size_of_RAM_byte = size_of_ram;
}


void FileProcessing::SizeRAM()
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        throw std::runtime_error("Couldn't open /proc/meminfo");
    }
    
    std::unordered_map<std::string, size_t> mem_data;
    std::string line;
    
    while (std::getline(meminfo, line)) {
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

    if (it != mem_data.end()) { 
        size_of_RAM_byte = it->second * 1024;
        return;
    }
}


void FileProcessing::LenInputFile() {
    struct stat st;
    if (fstat(input_descriptor, &st) != 0) {
        throw std::runtime_error("Couldn't get file size: ");
    }
    len_of_file_input = st.st_size;
}


void FileProcessing::EraseFromFile(size_t chunk_erase) {
    if (len_of_file_input == 0) {
        std::cout << "[INFO] Файл уже пуст\n";
        return;
    }

    size_t actual_erase = std::min(chunk_erase, len_of_file_input);

    off_t new_size = len_of_file_input - actual_erase;

    if (ftruncate(input_descriptor, new_size) == -1) {
        throw std::runtime_error("Ошибка усечения файла: " + std::string(strerror(errno)));
    }

    len_of_file_input = new_size;

    lseek(input_descriptor, 0, SEEK_END);

    std::cout << "[INFO] Удалено " << actual_erase << " байт с конца файла\n";
    std::cout << "[INFO] Новый размер файла: " << len_of_file_input << " байт\n";
}