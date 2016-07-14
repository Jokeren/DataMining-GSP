#include "database.h"
#include "gsp.h"
#include "gsp_hash_tree.h"
#include <cmath>

namespace gsp {
	bool DataBase::find_frequent_item_set(uint32_t nevents, uint32_t min_support) 
	{
		//To-do: check if the event id is [1, _m_nevents]
		std::vector<uint32_t> ncount(nevents + 1);

		for (sequences_iterator it = sequences.begin(); it != sequences.end(); ++it) {
			Sequence& sequence = it->second;
			std::set<uint32_t> s;

			for (size_t i = 0; i < sequence.size(); ++i) {
				item_set::iterator it;
				for (it = sequence[i].begin(); it != sequence[i].end();
						++it) {
					s.insert(it->id);
				}
			}

			for (std::set<uint32_t>::iterator s_it = s.begin(); s_it != s.end(); ++s_it) {
				++ncount[*s_it];
			}
		}

		for (size_t i = 1; i < ncount.size(); ++i) {
			if (ncount[i] >= min_support) {
				struct item_t item;
				item.id = i;
				item_set items;
				items.insert(item);

				Pattern *pattern = new Pattern(ncount[i], 1);
				pattern->push_back(items);
				patterns.push_back(*pattern);
			}
		}

		return true;
	}

	bool DataBase::create_patterns(uint32_t k, uint32_t nevents, uint32_t min_support, uint32_t min_gap, uint32_t max_gap) 
	{
		std::vector<Pattern> pattern_candidates;

		if (k == 2) {
			for (size_t i = 0; i < patterns.size(); ++i) {
				for (size_t j = i; j < patterns.size(); ++j) {
					if (j == i) {
						Pattern pattern;
						generate_two(pattern, patterns[i], patterns[j]);
						pattern_candidates.push_back(pattern);	

					} else {
						Pattern pattern_a, pattern_b, pattern_c;
						generate_two(pattern_a, pattern_b, pattern_c, patterns[i], patterns[j]);

						pattern_candidates.push_back(pattern_a);					
						pattern_candidates.push_back(pattern_b);	
						pattern_candidates.push_back(pattern_c);	
					}
				}
			}
		

		} else {
			for (size_t i = 0; i < patterns.size(); ++i) {
				for (size_t j = 0; j < patterns.size(); ++j) {
					//printf("check %s : %s\n", patterns[i].c_str(), patterns[j].c_str());
					if (!check_pattern(patterns[i], patterns[j])) {
						continue;
					}
					//use the default copy constructor, or problems might occur
					Pattern pattern(patterns[i]);
					generate(pattern, patterns[i], patterns[j]);			
					if (!prune(pattern)) {
						pattern_candidates.push_back(pattern);	
					}
				}
			}

		}

		//fprintf(stderr, "k : %d, candidate : %d, ", k,pattern_candidates.size());
		if (!pattern_candidates.empty()) {
			//TODO 100?
			if (pattern_candidates.size() < 20 ) {
				//fprintf(stderr, "%d,%d\n", k,pattern_candidates.size());
				count_candidates_with_brute_force(pattern_candidates, min_support, min_gap, max_gap);
			}
			else {
				count_candidates_with_hash_tree(pattern_candidates, k, nevents, min_gap, max_gap);
			}
		}

		patterns.clear();	
		for (size_t i = 0; i<pattern_candidates.size(); ++i) {
			if (pattern_candidates[i].get_support() >= min_support)
				patterns.push_back(pattern_candidates[i]);
		}
		//fprintf(stderr, "patterns : %d\n", patterns.size());

		if (patterns.size() == 0) {
			//no any candidate generate
			return false;
		} else {
			//start the next iteration
			return true;
		}
	}

	void DataBase::count_candidates_with_hash_tree(std::vector<Pattern>& pattern_candidates, uint32_t k, uint32_t nevents, uint32_t min_gap, uint32_t max_gap) 
	{
		Gsp_Hash_tree *gsp_Hash_tree = new Gsp_Hash_tree(1000,min_gap, max_gap);
		uint32_t hash_blocks_num = 150;
		uint32_t node_size = 500;

		if (k==2)
			hash_blocks_num = patterns.size() * 2;
		if (k==3)
			hash_blocks_num = 300;
		if (k==4)
			hash_blocks_num = 200;

		node_size = pattern_candidates.size() / pow((double)hash_blocks_num, (double)k);
		if (node_size<20)
			node_size = 20;

		//fprintf(stderr, "k : %d, hash : %d, %d\n", k,hash_blocks_num,node_size);

		gsp_Hash_tree->set_hash_blocks_num(hash_blocks_num);
		gsp_Hash_tree->set_node_size(node_size);

		gsp_Hash_tree->bulid_gsp_hash_tree(pattern_candidates);
		for (sequences_iterator it = sequences.begin(); it != sequences.end(); ++it) {
			gsp_Hash_tree->find_count_potential_patterns(it->second);
		}
		gsp_Hash_tree->delete_gsp_hash_tree();
	}

	void DataBase::count_candidates_with_brute_force(std::vector<Pattern>& pattern_candidates, uint32_t min_support, uint32_t min_gap, uint32_t max_gap)
	{
		for (size_t i = 0; i < pattern_candidates.size(); ++i) {
			uint32_t ncount = 0;
			if (count_pattern(pattern_candidates[i], min_support, min_gap, max_gap, ncount)) {
				//printf("new pattern count ncount %u\n", ncount);
				pattern_candidates[i].set_support(ncount);
			} 
		}
	}

	void DataBase::generate_two(Pattern& newpattern, const Pattern& pattern_a, const Pattern& pattern_b)
	{
		if (pattern_a.get_length() != 1 || pattern_b.get_length() != 1) {
			//To-do: report error!
			exit(-1);
		}
		item_set items;
		Pattern pattern;

		const item_set& items_a = pattern_a[0];	
		const item_set& items_b = pattern_b[0];
		
		struct item_t item_a = *(items_a.begin());
		struct item_t item_b = *(items_b.begin());

		//two item_set
		newpattern.push_back(items_a);
		newpattern.push_back(items_b);
		newpattern.set_length(2);
	}

	void DataBase::generate_two(Pattern& newpattern_a, Pattern& newpattern_b, Pattern& newpattern_c, 
			const Pattern& pattern_a, const Pattern& pattern_b)
	{
		if (pattern_a.get_length() != 1 || pattern_b.get_length() != 1) {
			//To-do: report error!
			exit(-1);
		}

		item_set items;

		const item_set& items_a = pattern_a[0];	
		const item_set& items_b = pattern_b[0];
		struct item_t item_a = *(items_a.begin());
		struct item_t item_b = *(items_b.begin());

		//one item_set
		items.insert(item_a);	
		items.insert(item_b);	
		newpattern_a.push_back(items);
		newpattern_a.set_length(2);

		//two item_set a first
		newpattern_b.push_back(items_a);
		newpattern_b.push_back(items_b);
		newpattern_b.set_length(2);

		//two item_set b first
		newpattern_c.push_back(items_b);
		newpattern_c.push_back(items_a);
		newpattern_c.set_length(2);
	}	

	void DataBase::generate(Pattern& pattern, 
			const Pattern& pattern_a, const Pattern& pattern_b) 
	{
		if (pattern_a.get_length() != pattern_b.get_length()) {
			//To-do: report error!
			exit(-1);
		}

		//To-do: implement the copy function
		const item_set& last_item_b = pattern_b[pattern_b.size() - 1];
		item_set::iterator it = last_item_b.begin();
		item_set::iterator it_next = last_item_b.begin();
		++it_next;

		//move the the last item
		while (it_next != last_item_b.end()) {
			++it_next;
			++it;
		}

		if (last_item_b.size() == 1) {
			item_set items;
			items.insert(*it);

			pattern.push_back(items);
		} else {
			//notice that there is no duplication, because the original size > 1
			//no duplication can ocurr in the set. 
			pattern[pattern.size() - 1].insert(*it);
		}

		pattern.set_length(pattern.get_length() + 1);
		pattern.set_support(0);
	}	
}//namespace gsp
