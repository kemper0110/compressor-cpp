cmake_minimum_required(VERSION 3.28)
project(compressor)

set(CMAKE_CXX_STANDARD 23)
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/boost)

include_directories(include)
add_library(compressor STATIC src/BitReader.cpp src/BitWriter.cpp src/HuffmanCompressor.cpp src/HuffmanDecompressor.cpp src/HuffmanTree.cpp
        src/LZSSCompressor.cpp src/LZSSDecompressor.cpp)

target_include_directories(compressor PRIVATE
        ${BOOST_ROOT}/core/include
        ${BOOST_ROOT}/type_traits/include
        ${BOOST_ROOT}/config/include
        ${BOOST_ROOT}/static_assert/include
        ${BOOST_ROOT}/throw_exception/include
        ${BOOST_ROOT}/concept_check/include
        ${BOOST_ROOT}/preprocessor/include
        ${BOOST_ROOT}/assert/include
        ${BOOST_ROOT}/move/include
        ${BOOST_ROOT}/circular_buffer/include
)