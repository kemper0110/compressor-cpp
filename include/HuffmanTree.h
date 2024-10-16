#pragma once

#include <istream>
#include <limits>
#include <vector>
#include <cstdint>
#include <array>

namespace Huffman {
	using Sign = uint8_t;
	static constexpr unsigned long long MAX_SIGN_VALUE = std::numeric_limits<Sign>::max() + 1;

	struct Header {
		char type[2]{ 'H', 'M' };				// 2 bytes
		uint16_t dict_size{};					// 2 bytes
		uint32_t byte_size{};
	};

	struct SignWidth {
		Sign sign;
		uint8_t code_width;
	};


	struct SignFrequency {
		Sign sign{};
		uint32_t freq{};
	};



	enum class TreeType {
		Compressor, Decompressor
	};




	struct CompressorNode
		// debatable but useful
		: SignFrequency
	{
		CompressorNode* children[2]{
			nullptr, nullptr
		};
	};
	struct DecompressorNode 
		: SignWidth
	{
		DecompressorNode* children[2]{
			nullptr, nullptr
		};
	};


	std::tuple<std::vector<CompressorNode>, std::vector<CompressorNode>, CompressorNode*>
		huffmanImpl(std::vector<SignFrequency>& frequency_vec);

	std::pair<
		std::vector<SignFrequency>,
		std::array<std::vector<bool>, MAX_SIGN_VALUE>
	>
		makeCompressorTree(std::istream& istream);



	std::tuple<std::vector<DecompressorNode>, std::vector<DecompressorNode>, DecompressorNode*>
		makeDecompressorTree(std::vector<SignFrequency>& frequency_vec);
}
