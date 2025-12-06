add_library(FileProcessing
    FileProcessing.h
    FileProcessing.cpp
)

target_include_directories(FileProcessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})