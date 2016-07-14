#ifndef GSP_HASH_TREE_H
#define GSP_HASH_TREE_H
#include "common.h"
#include "database.h"
#include "gsp.h"

namespace gsp{
	struct patterns_node_t;
	typedef struct patterns_node_t patterns_node;

	class Gsp_Hash_tree {
		public:
			Gsp_Hash_tree() : 
			  node_size(100), hash_blocks_num(10), min_gap(0), max_gap(1), hash_tree_root(NULL), max_sequence_length(100){
				database = DataBase::get_instance();
			}

			Gsp_Hash_tree(uint32_t _max_seq_length, uint32_t _min_gap, uint32_t _max_gap) : 
			node_size(100), hash_blocks_num(10), min_gap(_min_gap), max_gap(_max_gap), hash_tree_root(NULL), max_sequence_length(_max_seq_length) {
				database = DataBase::get_instance();
			}

			Gsp_Hash_tree(uint32_t _node_size, uint32_t _hash_blocks_num, uint32_t _max_seq_length, uint32_t _min_gap, uint32_t _max_gap) : 
				node_size(_node_size), hash_blocks_num(_hash_blocks_num), max_sequence_length(_max_seq_length), min_gap(_min_gap), max_gap(_max_gap), hash_tree_root(NULL) {
				database = DataBase::get_instance();
			}

			void set_node_size(const uint32_t _node_zise) {
				node_size = _node_zise;
			}

			void set_hash_blocks_num(const uint32_t _hash_blocks_num) {
				hash_blocks_num = _hash_blocks_num;
			}

			void bulid_gsp_hash_tree(std::vector<Pattern>& patterns);

			void find_count_potential_patterns(const Sequence& sequence);

			void delete_gsp_hash_tree();
		private:
			void split_node(patterns_node *interior_node, size_t item_idx);

			void split_node(patterns_node *interior_node, const std::vector<Pattern *>& node_patterns, size_t item_idx);

			void check_node_with_subsequence(patterns_node *interior_node, size_t depth, 
					const Sequence& sequence, size_t pri_item_set_idx, size_t pri_item_idx, size_t sequence_item_idx);

			void delete_node(patterns_node *tree_node, std::string prefix);

			void reset_count_state();

			void print_tree(patterns_node *p, std::string prefix);
		private:
			uint32_t node_size;
			uint32_t hash_blocks_num;
			uint32_t patern_length;
			uint32_t min_gap;
			uint32_t max_gap;
			uint32_t max_sequence_length;
			
			patterns_node *hash_tree_root;
			DataBase *database;
	};

	struct patterns_node_t {
		//do not free this!
		std::vector<Pattern *> node_patterns;
		//need to be freed
		std::vector<struct patterns_node_t *> children;
		//record the sequence_item_idx
		std::vector<int32_t> visited;

		int32_t checked;

		patterns_node_t() {};
	};
}//namespace gsp

#endif//GSP_HASH_TREE
