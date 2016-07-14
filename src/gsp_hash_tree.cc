#include <sstream>
#include "gsp_hash_tree.h"


namespace gsp{

	void Gsp_Hash_tree::bulid_gsp_hash_tree(std::vector<Pattern>& patterns)
	{
		if (hash_tree_root != NULL) 
			delete_gsp_hash_tree();

		if (patterns.empty())
			return;
		//clear the state

		this->patern_length = patterns[0].get_length();

		hash_tree_root = new patterns_node();
		hash_tree_root->node_patterns.reserve(patterns.size());

		for (size_t i = 0; i < patterns.size(); ++i)
			hash_tree_root->node_patterns.push_back(&patterns[i]);
		split_node(hash_tree_root, 0);
	}

	void Gsp_Hash_tree::split_node(patterns_node *interior_node, size_t item_idx)
	{
		if (interior_node->node_patterns.size() <= node_size || item_idx >= patern_length) {
			interior_node->checked = -1;
			return;
		}
		else {
			interior_node->children =  std::vector<struct patterns_node_t *>(hash_blocks_num, NULL);

			//distribute patterns to children
			for (size_t i = 0; i < interior_node->node_patterns.size(); ++i) {
				//the item with index of item_idx
				uint32_t hashed_item_id = interior_node->node_patterns[i]->items_list().at(item_idx);
				uint32_t hash_key = hashed_item_id % hash_blocks_num;
				//malloc the space if the pointer is null
				if (interior_node->children[hash_key] == NULL)
				{
					interior_node->children[hash_key] = new patterns_node();
					//init the children as NULL
					interior_node->children[hash_key]->node_patterns.reserve(node_size);
					interior_node->visited.resize(max_sequence_length, -1);

				}
				interior_node->children[hash_key]->node_patterns.push_back(interior_node->node_patterns[i]);
			}
			//release the space of node_patterns, but not release what they point
			std::vector<Pattern *>().swap(interior_node->node_patterns);

			//split the children
			for (size_t i = 0; i < hash_blocks_num; ++i) {
				if(interior_node->children[i] != NULL)
					split_node(interior_node->children[i], item_idx + 1);
			}

		}
	}

	void Gsp_Hash_tree::delete_gsp_hash_tree()
	{
		delete_node(hash_tree_root, "r");
	}

	void Gsp_Hash_tree::delete_node(patterns_node *tree_node, std::string prefix)
	{

		if (tree_node == NULL)
			return;
		//release the not empty node_patterns
		if (!tree_node->node_patterns.empty()) {
			//std::vector<Pattern *>().swap(*tree_node->node_patterns);
			tree_node->node_patterns.clear();
		}

		//free children
		if (!tree_node->children.empty()) {
			for (size_t i = 0; i < tree_node->children.size(); ++i) {
				std::stringstream ss;
				ss << i;
				if (tree_node->children[i] != NULL)
					delete_node(tree_node->children[i], prefix + ss.str());
			}
		}
		tree_node->children.clear();

		//free this node
		delete tree_node;
		//fprintf(stderr,(prefix+"\n").c_str());
		tree_node = NULL;
	}

	void Gsp_Hash_tree::print_tree(patterns_node *p, std::string prefix)
	{
		if(p != NULL)
		{
			if(p->node_patterns.size()>0)
			{
				std::stringstream ss;
				ss << "]";
				prefix = prefix + ss.str();
				printf("%s : ",prefix.c_str());
				printf("%zu",p->node_patterns.size());
				//for (size_t i =0; i<p->node_patterns->size(); ++i)
				//		printf("%s,  ", p->node_patterns->at(i)->c_str());
				printf(";\n");
			}
			else
			{
				for (size_t i =0; i<p->children.size(); ++i) {
					std::stringstream ss;
					ss << i << ", ";
					print_tree(p->children[i], prefix+ss.str());
				}
			}
		}
	}


	void Gsp_Hash_tree::find_count_potential_patterns(const Sequence& sequence)
	{
		//root, depth=0, sequence, item_set_idx = 0, item_idx=-1 is ok
		check_node_with_subsequence(hash_tree_root, 0, sequence, 0, 0, 0);
	}

	void Gsp_Hash_tree::check_node_with_subsequence(patterns_node *interior_node, size_t depth, 
			const Sequence& sequence, size_t pri_item_set_idx, size_t pri_item_idx, size_t sequence_item_idx)
	{
		if (interior_node==NULL)
			return;

		if (!interior_node->node_patterns.empty()) {
			for (size_t i = 0; i < interior_node->node_patterns.size(); ++i) {
				if (database->check_pattern_sequence( *(interior_node->node_patterns[i]), sequence, this->min_gap, this->max_gap)) {
					interior_node->node_patterns[i]->increase_support();
				}
			}
			/*leaf_nodes[pointer_string] = true;*/
			return;
		}

		uint32_t pri_time = sequence[pri_item_set_idx].timestamp;

		for (size_t item_set_id = pri_item_set_idx; item_set_id < sequence.size(); ++item_set_id) {
			//break when exceed the max_gap
			if(depth != 0 && sequence[item_set_id].timestamp - pri_time > max_gap)
				break;

			//locate the iterator of the item after pri_item
			item_set::iterator item_it = sequence[item_set_id].begin();
			size_t idx = 0;
			if (item_set_id == pri_item_set_idx && depth != 0) {
				for(; item_it != sequence[item_set_id].end() && idx <= pri_item_idx; ++item_it)
					++idx;
				//if pri_item is the last item in current item_set, find next item_set
				if(item_it == sequence[item_set_id].end())
					continue;
			}

			//iterate from here, 
			for (; item_it != sequence[item_set_id].end(); ++item_it, ++idx) {
				++sequence_item_idx;
				uint32_t hash_key = item_it->id % hash_blocks_num;

				if (interior_node->children[hash_key] == NULL)
					continue;

				//leaf nodes do not need its own hashmap
				if (!interior_node->children[hash_key]->node_patterns.empty()) {

					if (interior_node->children[hash_key]->checked != static_cast<int32_t>(sequence.get_sequence_id())) {
						interior_node->children[hash_key]->checked = static_cast<int32_t>(sequence.get_sequence_id());
						check_node_with_subsequence(interior_node->children[hash_key], depth+1, sequence, item_set_id, idx, sequence_item_idx);
					}
				}
				else {
					//first judge, then fetch hashtable
					if ( interior_node->children[hash_key]->visited[sequence_item_idx] != sequence.get_sequence_id()) {
						interior_node->children[hash_key]->visited[sequence_item_idx] = sequence.get_sequence_id();
						check_node_with_subsequence(interior_node->children[hash_key], depth+1, sequence, item_set_id, idx, sequence_item_idx);
					}
				}
			}
		}
	}
}//namespace gsp
