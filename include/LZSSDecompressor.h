#pragma once

#include <istream>
#include <ostream>

namespace LZSS {
    class LZSSDecompressor {
    public:
        LZSSDecompressor();

        void decompress(std::istream &istream, std::ostream &ostream);
    };
}