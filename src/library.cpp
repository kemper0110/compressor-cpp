//#include "../include/library.h"
//#include "../include/LZSSCompressor.h"
//#include "../include/LZSSDecompressor.h"
//#include "../include/HuffmanCompressor.h"
//#include "../include/HuffmanDecompressor.h"
//
//#include <iostream>
//#include <ranges>
//#include <thread>
//#include <sstream>
//#include <fstream>
//
//
//std::string HSCompress(const std::string& input, int dict_size, int buffer_size){
//    std::stringstream iss(input), pss, oss;
//
//    Huffman::HuffmanCompressor().compress(iss, pss);
//    LZSS::LZSSCompressor(dict_size, buffer_size).compress(pss, oss);
//
//    std::string compress_result = oss.str();
//    return compress_result;
//}
//
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