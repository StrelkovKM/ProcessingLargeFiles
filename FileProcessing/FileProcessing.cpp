#include "FileProcessing.h"

FileProcessing::FileProcessing(const std::string& input_filename_, const std::string& output_filename_)
    : RAM(), input_descriptor(-1), output_descriptor(-1), size_of_RAM(0)
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


void FileProcessing::EraseFromFile(size_t chunk_erase) {
    if (len_input_file == 0) {
        std::cout << "[INFO] The file is already empty\n";
        return;
    }

    size_t actual_erase = std::min(chunk_erase, len_input_file);

    off_t new_size = len_input_file - actual_erase;

    if (ftruncate(input_descriptor, new_size) == -1) {
        throw std::runtime_error("File Truncation error: " + std::string(strerror(errno)));
    }

    len_input_file = new_size;

    lseek(input_descriptor, 0, SEEK_END);

    std::cout << "[INFO] Deleted: " << actual_erase << " byte\n";
    std::cout << "[INFO] New file size: " << len_input_file << " byte\n";
}

void FileProcessing::WriteToFile(int file_descriptor)
{
    if (RAM.empty()) {
        return;
    }
    
    if (lseek(output_descriptor, 0, SEEK_END) == -1) {
        throw std::runtime_error("Positioning error in the output file");
    }
    
    ssize_t bytes_written = write(output_descriptor, RAM.data(), RAM.size());
    if (bytes_written == -1) {
        throw std::runtime_error("Recording error: " + std::string(strerror(errno)));
    }
    
    fsync(output_descriptor);
    
    std::cout << "Writen to new file: " << bytes_written << "byte" << std::endl;
}
