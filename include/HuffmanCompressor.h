#pragma once

#include <istream>
#include <ostream>

#include "HuffmanTree.h"


namespace Huffman {
	class HuffmanCompressor
	{
	public:
		HuffmanCompressor();
		void compress(std::istream& istream, std::ostream& ostream);
	private:
	};
}

