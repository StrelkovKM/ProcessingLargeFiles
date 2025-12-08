#include "FileProcessing.h"



FileProcessing::FileProcessing(const std::string& name_of_file_input_, const std::string& name_of_file_output_)
    : RAM(), input_file(name_of_file_input_), output_file(name_of_file_output_), size_of_RAM_byte(0)
{
    this->SizeRAM();
    std::cout << "RAM:" << size_of_RAM_byte << "Byte" << "\n";
    this->LenInputFile();
    if (!input_file.is_open() && !output_file.is_open())
        throw std::runtime_error("Couldn't open this file");
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
    // 1. For new kernels
    if (it != mem_data.end()) { 
        size_of_RAM_byte = it->second;
        return;
    } else {
    // 2. For past kernels
    size_t available_kb = 0;
    
    if (mem_data.find("MemFree") == mem_data.end()) {
        throw std::runtime_error("Не удалось найти MemFree в /proc/meminfo");
    }
    
    available_kb = mem_data["MemFree"];

    if (mem_data.find("Buffers") != mem_data.end()) {
        available_kb += mem_data["Buffers"];
    }
    
    if (mem_data.find("Cached") != mem_data.end()) {
        available_kb += mem_data["Cached"];
    }
    
    if (mem_data.find("SReclaimable") != mem_data.end()) {
        available_kb += mem_data["SReclaimable"];
    }
    
    if (mem_data.find("SUnreclaim") != mem_data.end()) {
        if (available_kb > mem_data["SUnreclaim"]) {
            available_kb -= mem_data["SUnreclaim"];
        }
    }
    
    if (available_kb == 0) {
        throw std::runtime_error("Free RAM: 0");
    }
    
    size_of_RAM_byte = available_kb * 1024;
    }
}


void FileProcessing::LenInputFile()
{
    std::streampos current_pos = input_file.tellg();
    input_file.seekg(0, std::ios::end);
    size_t size = input_file.tellg();
    input_file.seekg(current_pos);
    len_of_file_input = size;
    std::cout << "File size: " << size << " B\n";
}