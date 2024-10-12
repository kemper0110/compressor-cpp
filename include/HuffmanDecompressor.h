#pragma once

#include <istream>
#include <ostream>

#include "HuffmanTree.h"

namespace Huffman {
    class HuffmanDecompressor {
    public:
        HuffmanDecompressor();

        void decompress(std::istream &istream, std::ostream &ostream);

    private:
        Header readHeader(std::istream &istream);
    };
}
