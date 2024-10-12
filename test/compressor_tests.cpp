//
// Created by Danil on 12.10.2024.
//
#include <gtest/gtest.h>
#include <iostream>
#include <ranges>
#include <sstream>
#include <fstream>
#include "HuffmanCompressor.h"
#include "LZSSCompressor.h"

TEST(BasicCompressText, Mumu) {
    std::ifstream ifs("../test/mumu.txt");
    if (!ifs.is_open())
        throw std::runtime_error("File not found");
    ifs.seekg(0, std::ios::end);
    const auto size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::cout << "initial size is: " << size << '\n';

    std::stringstream pss, oss;
    Huffman::HuffmanCompressor().compress(ifs, pss);
    LZSS::LZSSCompressor(256, 2048).compress(pss, oss);

    std::string compress_result = oss.str();
    std::cout << "compressed size: " << compress_result.size() << '\n';

    const auto reduce = (double) compress_result.size() / size - 1.0;
    std::cout << reduce * 100 << "% file size lost" << '\n';
    // expected loss: ~35%
}


//std::string HSDecompress(const std::string& input){
//    std::stringstream iss(input), pss, oss;
//
//    LZSS::LZSSDecompressor().decompress(iss, pss);
//    Huffman::HuffmanDecompressor().decompress(pss, oss);
//
//    std::string decompress_result = oss.str();
//    return decompress_result;
//}
//
//std::string HuffmanDecompress(std::string input){
//    std::stringstream iss(input);
//    std::stringstream oss;
//    Huffman::HuffmanDecompressor().decompress(iss, oss);
//
//    std::string decompress_result = oss.str();
//    return decompress_result;
//}
//
//std::string HuffmanCompress(std::string input){
//    std::stringstream iss(input);
//    std::stringstream oss;
//    Huffman::HuffmanCompressor().compress(iss, oss);
//
//    std::string compress_result = oss.str();
//    return compress_result;
//}
//
//std::string LZSSCompress(std::string input, int dict_size, int buffer_size) {
//    const auto begin = std::chrono::steady_clock::now();
//    std::stringstream iss(input);
//    std::stringstream oss;
//    LZSS::LZSSCompressor(dict_size, buffer_size).compress(iss, oss);
//    const auto end = std::chrono::steady_clock::now();
//
//    std::string compress_result = oss.str();
//    std::cout << "compress size: " << compress_result.size() << '\n';
//    std::cout << "elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << '\n';
//
//    return compress_result;
//}
//
//std::string LZSSDecompress(std::string input) {
//    const auto begin = std::chrono::steady_clock::now();
//    std::stringstream iss(input);
//    std::stringstream oss;
//    LZSS::LZSSDecompressor().decompress(iss, oss);
//    const auto end = std::chrono::steady_clock::now();
//
//    std::string decompress_result = oss.str();
//    std::cout << "decompressed size: " << decompress_result.size() << '\n';
//    std::cout << "elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << '\n';
//
//
//    return decompress_result;
//}