cmake_minimum_required(VERSION 2.8)
project( Preprocessing )

find_package( OpenCV REQUIRED )
find_package( OpenMP REQUIRED )

add_executable( DisplayImage DisplayImage.cpp )
add_executable( preprocess_serial preprocess_serial.cpp )

target_link_libraries( DisplayImage ${OpenCV_LIBS} )
target_link_libraries( preprocess_serial ${OpenCV_LIBS} )

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
     message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
