add_executable(src main.cpp)
target_link_libraries(src PRIVATE FileProcessing)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/InputFile.txt 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/OutputFile.txt 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})