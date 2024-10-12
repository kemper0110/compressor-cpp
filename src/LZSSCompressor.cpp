#include "LZSSCompressor.h"
#include <boost/circular_buffer.hpp>
#include <algorithm>
#include <bit>
#include "BitWriter.h"

namespace LZSS {
	LZSSCompressor::LZSSCompressor(size_t dict_size, size_t buffer_size) : dict_size(dict_size), buffer_size(buffer_size)
	{
	}
	LZSSCompressor::LZSSCompressor() = default;
	LZSSCompressor& LZSSCompressor::setDictSize(size_t dict_size)
	{
		this->dict_size = dict_size;
		return *this;
	}
	LZSSCompressor& LZSSCompressor::setBufferSize(size_t buffer_size)
	{
		this->buffer_size = buffer_size;
		return *this;
	}
	size_t LZSSCompressor::getDictSize()
	{
		return dict_size;
	}
	size_t LZSSCompressor::getBufferSize()
	{
		return buffer_size;
	}

	void LZSSCompressor::compress(std::istream& istream, std::ostream& ostream)
	{
		if (dict_size == 0 or buffer_size == 0)
            std::exit(1);

		boost::circular_buffer<unsigned char> window(dict_size + buffer_size, dict_size, 0);

		const auto
			max_pos_width = std::bit_width(dict_size),
			max_len_width = std::bit_width(buffer_size);
		const auto refweight = 1 + max_pos_width + max_len_width;


		const auto dict = [this, &window] {
			return std::pair{ window.begin(), window.begin() + dict_size };
		};
		const auto buffer = [this, &window] {
			return std::pair{ window.begin() + dict_size, window.end() };
		};

		/*
			tree				    ->		 std::set<string>
			queue with indices      ->		 circular_buffer<set::iterator>

			when pushing new sign to dictionary
			->	take iterator to oldest in dictionary from queue	| front()     |
			->	extract node of oldest								| extract()   |
			->  change value to new sign							| value()	  |
			->  push to set and take iter							| insert()	  |
			->  push_back iter to queue								| push_back() |
		*/

		LZSSHeader header{
			//.byte_size = (uint32_t)byte_size,
			.dict_size = (uint32_t)this->dict_size,
			.buffer_size = (uint32_t)this->buffer_size,
			.pos_width = (uint8_t)max_pos_width,
			.len_width = (uint8_t)max_len_width,
		};
		ostream.write(reinterpret_cast<const char*>(&header), sizeof header);


		const auto nextChar = [&istream]() -> std::optional<unsigned char> {
			char c;
			return istream.get(c) ? std::optional{ c } : std::nullopt;
		};
		for (auto i = 0UL; i < buffer_size; ++i)
			if (const auto sign = nextChar(); sign.has_value())
				window.push_back(sign.value());
			else
				break;

		{
			BitWriter bitwriter(ostream);

			// aka while buffer is not empty
			while (buffer().first != buffer().second) {
				using Iter = decltype(window.begin());

				const auto [sub_begin, sub_end] = buffer();
				const auto [begin, end] = dict();

				const auto ff = [&sub_begin = sub_begin, &sub_end = sub_end](Iter begin, Iter end) {
					auto [i1, i2] = std::mismatch(begin, end, sub_begin, sub_end);
					return std::tuple{
						begin, i1, i2
					};
				};

				auto best_ref = std::pair{ end, std::ptrdiff_t{} };
				for (auto [prev_i1, i1, i2] = ff(begin, end);
					i1 != end;
					std::tie(prev_i1, i1, i2) = ff(i1 + 1, end)) 
				{
					const auto match_size = std::distance(sub_begin, i2);
					if (match_size > best_ref.second)
						best_ref = std::pair{ prev_i1, match_size };
				}

				for (auto [i1, i2] = std::mismatch(begin, end, sub_begin, sub_end);
					i1 != end;
					std::tie(i1, i2) = std::mismatch(i1 + 1, end, sub_begin, sub_end))
				{
					const auto match_size = std::distance(sub_begin, i2);
					if (match_size > best_ref.second)
						best_ref = std::pair{ i1, match_size };
				}
				assert(best_ref.second >= 0);

				const auto literalweight = (1ULL + 8ULL) * best_ref.second;		// for 1 byte = 8 bit

				// write literal(s)
				if (literalweight <= refweight) {
					// if ref was not found
					if (best_ref.second == 0)
						best_ref.second = 1;
					for (int i = 0; i < best_ref.second; ++i) {
						//output.push_back(Element{ *buffer().first });
						bitwriter.write(0, 1);
						bitwriter.write(*buffer().first, sizeof * buffer().first * 8);
						//debug << *buffer().first << std::endl;
						if (const auto sign = nextChar(); sign.has_value())
							window.push_back(sign.value());
						else
							window.pop_front();
					}
				}
				// write reference
				else {
					const auto pos = std::distance(dict().first, best_ref.first);
					//output.push_back(Element{ (uint32_t)pos, (uint32_t)best_ref.second });
					bitwriter.write(1, 1);
					bitwriter.write(pos, max_pos_width);
					bitwriter.write(best_ref.second, max_len_width);

					for (int i = 0; i < best_ref.second; ++i)
						if (const auto sign = nextChar(); sign.has_value())
							window.push_back(sign.value());
						else
							window.pop_front();
				}
			}
		}

	}
}
