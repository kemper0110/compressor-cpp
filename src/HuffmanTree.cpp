#include "HuffmanTree.h"

#include <algorithm>
#include <array>

namespace Huffman
{
	std::pair<
		std::vector<SignFrequency>,
		std::array<std::vector<bool>, MAX_SIGN_VALUE>
	>
		makeCompressorTree(std::istream& istream) {
		std::array<uint32_t, MAX_SIGN_VALUE> frequency_map{};

		std::for_each(std::istreambuf_iterator<char>{istream}, {}, [&frequency_map](char c) {
			const auto sign = static_cast<Sign>(c);
			frequency_map[sign]++;
			}
		);

		std::vector<SignFrequency> frequency_vec;
		frequency_vec.reserve(frequency_map.size());
		for (auto i = 0UL; i < frequency_map.size(); ++i)
			if (frequency_map[i])
				frequency_vec.push_back({static_cast<Sign>(i), frequency_map[i]});

		std::sort(frequency_vec.begin(), frequency_vec.end(), [](const SignFrequency& lhs, const SignFrequency& rhs) {
			if (lhs.freq < rhs.freq)
				return true;
			if (lhs.freq > rhs.freq)
				return false;
			if (lhs.sign < rhs.sign)
				return true;
			return false;
			});

		const auto [leafs, inner_nodes, root] = huffmanImpl(frequency_vec);


		std::array<std::vector<bool>, MAX_SIGN_VALUE> codemap;
		auto recursive_mapping = [&codemap](const CompressorNode* root, auto&& continuation, std::vector<bool>& vec) {
			if (root == nullptr) return;
			if (root->children[0] and root->children[1]) {
				vec.push_back(0);
				continuation(root->children[0], continuation, vec);
				vec.back() = 1;
				continuation(root->children[1], continuation, vec);
				vec.pop_back();
			}
			else {
				codemap[root->sign] = vec;
			}
		};
		std::vector<bool> v;
		v.reserve(256);
		recursive_mapping(root, recursive_mapping, v);

		return std::pair{ frequency_vec, codemap };
	}

	std::tuple<std::vector<CompressorNode>, std::vector<CompressorNode>, CompressorNode*>
		huffmanImpl(std::vector<SignFrequency>& frequency_vec) {

		auto leafs = std::vector<CompressorNode>();
        for(const auto& sf : frequency_vec)
            leafs.push_back(CompressorNode{sf});

		auto inner_nodes = std::vector<CompressorNode>(
			frequency_vec.size(),
			CompressorNode{ 0, std::numeric_limits<uint32_t>::max() / 2 }
		);


		const auto getMin = [](std::vector<CompressorNode>& nodelist, uint32_t i) -> std::pair<CompressorNode&, CompressorNode&> {
			static auto null = CompressorNode{ 0, std::numeric_limits<uint32_t>::max() / 2 };
			switch (nodelist.size() - i) {
			case 0:
				return { null, null };
			case 1:
				return { nodelist[i], null };
			default:
				return { nodelist[i], nodelist[size_t(i) + 1] };
			}
		};


		CompressorNode* root = nullptr;
		uint32_t i = 0, j = 0;
		for (auto k = 0UL; k < inner_nodes.size() - 1; ++k) {
			const auto [leaf1, leaf2] = getMin(leafs, i);
			const auto [node1, node2] = getMin(inner_nodes, j);
			const auto
				s1 = leaf1.freq + leaf2.freq,
				s2 = node1.freq + node2.freq,
				s3 = leaf1.freq + node1.freq;
			if (s1 <= s2 && s1 <= s3) {
				inner_nodes[k] = CompressorNode{ 0, s1, &leaf1, &leaf2 };
				i += 2;
			}
			else if (s2 <= s1 && s2 <= s3) {
				inner_nodes[k] = CompressorNode{ 0, s2, &node1, &node2 };
				j += 2;
			}
			else if (s3 <= s1 && s3 <= s2) {
				inner_nodes[k] = CompressorNode{ 0, s3, &leaf1, &node1 };
				i++; j++;
			}
			else
                std::exit(1);
			root = &inner_nodes[k];
		}


		return std::make_tuple(std::move(leafs), std::move(inner_nodes), root);
	}
}
