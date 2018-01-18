cmake_minimum_required(VERSION 3.8)
project(poo_alpr)

find_package( OpenCV REQUIRED )

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp main.h DetectChars.cpp DetectChars.h DetectPlates.cpp DetectPlates.h PossibleChar.cpp PossibleChar.h PossiblePlate.cpp PossiblePlate.h Preprocess.cpp Preprocess.h)

add_executable(poo_alpr ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries( poo_alpr ${OpenCV_LIBS})

file(COPY classifications.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY images.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})