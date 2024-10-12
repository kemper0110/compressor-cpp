#include "LZSSDecompressor.h"

#include <boost/circular_buffer.hpp>
#include "BitReader.h"
#include "LZSSHeader.h"

namespace LZSS {
    LZSSDecompressor::LZSSDecompressor() = default;

    void LZSSDecompressor::decompress(std::istream &istream, std::ostream &ostream) {

        LZSSHeader header;
        /*{
            .dict_size = 9,
            .buffer_size = 5,
            .pos_width = 4,
            .len_width = 3,
        };*/
        if (not istream.read(reinterpret_cast<char *>(&header), sizeof header)) {
            std::exit(1);
        }

        if (std::string_view(header.type, 2) != "SS") {
            std::exit(1);
        }


        BitReader bitreader(istream);

        boost::circular_buffer<unsigned char> dict(header.dict_size, header.dict_size, 0);

        for (auto is_ref = bitreader.read(1); is_ref.has_value(); is_ref = bitreader.read(1)) {
            switch (is_ref.value()) {
                case 0: {
                    const auto literal_opt = bitreader.read(8);
                    if(not literal_opt.has_value())
                        break;
                    const auto literal = static_cast<unsigned char>(literal_opt.value());
                    ostream.write(reinterpret_cast<const char *>(&literal), 1);
                    dict.push_back(literal);
                    break;
                }
                case 1: {
                    const auto pos_opt = bitreader.read(header.pos_width);
                    if(not pos_opt.has_value())
                        break;
                    const auto pos = pos_opt.value();
                    const auto len_opt = bitreader.read(header.len_width);
                    if(not len_opt.has_value())
                        break;
                    const auto len = len_opt.value();

                    for (auto j = 0UL; j < len; ++j) {
                        const auto sign = dict.at(pos);
                        ostream.write(reinterpret_cast<const char *>(&sign), 1);
                        dict.push_back(sign);
                    }
                    break;
                }
            }
        }
    }

}