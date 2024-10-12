#include "HuffmanCompressor.h"
#include <iostream>
#include <algorithm>
#include "BitWriter.h"
#include <array>

namespace Huffman {
	HuffmanCompressor::HuffmanCompressor() = default;

	void HuffmanCompressor::compress(std::istream& istream, std::ostream& ostream)
	{
		auto [frequency_vec, codemap] = makeCompressorTree(istream);

		istream.seekg(0, istream.beg);

		Header header = {
			.dict_size = (decltype(Header::dict_size))frequency_vec.size(),
			.byte_size = 0
		};
		std::cout << "dict size: " << frequency_vec.size() << '\n';
		ostream.write(reinterpret_cast<const char*>(&header), sizeof header);

		for (const auto& sf : frequency_vec) {
			ostream.write(reinterpret_cast<const char*>(&sf.sign), sizeof sf.sign);
			ostream.write(reinterpret_cast<const char*>(&sf.freq), sizeof sf.freq);
		}

		{
			BitWriter bitwriter(ostream);
			std::for_each(std::istreambuf_iterator<char>(istream), {}, [&codemap = codemap, &bitwriter](char c) {
				const auto sign = static_cast<Sign>(c);
				const auto& code = codemap[sign];
				for (const auto bit : code)
					bitwriter.write(bit, 1);
				});
		}
	}
}

