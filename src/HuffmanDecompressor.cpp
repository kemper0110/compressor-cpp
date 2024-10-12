#include "HuffmanDecompressor.h"

#include "BitReader.h"
#include <array>
#include <bitset>

namespace Huffman {
    HuffmanDecompressor::HuffmanDecompressor() = default;

    void HuffmanDecompressor::decompress(std::istream &istream, std::ostream &ostream) {
        const auto header = readHeader(istream);

        std::vector<SignFrequency> frequency_vec;
        frequency_vec.reserve(header.dict_size);
        for (int i = 0; i < header.dict_size; ++i) {
            SignFrequency sf;
            istream.read(reinterpret_cast<char *>(&sf.sign), sizeof sf.sign);
            istream.read(reinterpret_cast<char *>(&sf.freq), sizeof sf.freq);
            frequency_vec.push_back(sf);
        }
        auto [leafs, inner_nodes, root] = huffmanImpl(frequency_vec);

        {
            BitReader bitreader(istream);

            const auto *current = root;
            const auto isLeaf = [&root = root](const auto *node) {
                return node->children[0] == nullptr or node->children[1] == nullptr;
            };
            while (true) {
                const auto bit_opt = bitreader.read(1);
                if (not bit_opt.has_value())
                    break;
                const auto bit = bit_opt.value();
                current = current->children[bit];
                if (isLeaf(current)) {
                    ostream.write(reinterpret_cast<const char *>(&current->sign), 1);
                    current = root;
                }
            }

        }

    }

    Header HuffmanDecompressor::readHeader(std::istream &istream) {
        Header header;
        if (not istream.read(reinterpret_cast<char *>(&header), sizeof(Header)))
            std::exit(1);

        if (std::string_view(header.type, 2) != "HM")
            std::exit(1);

        return header;
    }
}


