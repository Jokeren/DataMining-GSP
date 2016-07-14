#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"

namespace gsp {
	class Pattern;
	class Sequence;
	struct item_set_t;
	typedef struct item_set_t item_set;

	//singleton
	class DataBase {
		public:
			static DataBase *get_instance() {
				return _m_instance;
			}

			void sort_sequences();

			bool find_frequent_item_set(uint32_t nevents, uint32_t _m_support);

			bool create_patterns(uint32_t k, uint32_t nevents, uint32_t min_support, uint32_t min_gap = 0, uint32_t max_gap = 0x7fffffff);

			//push_back is defined as copyable(const argument) or movable!!!
			//the declaration is consistent with it
			void push_item_set(const item_set& items, uint32_t sequence_id);

			void print_patterns();

			bool check_pattern_sequence(const Pattern& candidate, const Sequence& sequence, uint32_t min_gap, uint32_t max_gap);

			bool check_pattern_sequence_time_list(const Pattern& candidate, const Sequence& sequence, uint32_t min_gap, uint32_t max_gap);

			void set_max_sequence_length(uint32_t length) 
			{
				_m_max_sequences_length = length;		
			}

			uint32_t get_max_sequence_length() const 
			{
				return _m_max_sequences_length;
			}

		private:
			bool check_pattern(const Pattern& pattern_a, const Pattern& pattern_b);

			bool prune(const Pattern& pattern);

			bool match_pattern(const Pattern& pattern_a, const Pattern& pattern_b);

			bool match_item_set(const item_set& items_a, const item_set& items_b);

			bool count_pattern(const Pattern& pattern, uint32_t min_support, uint32_t min_gap, uint32_t max_gap, uint32_t& ncount);

			void count_candidates_with_hash_tree(std::vector<Pattern>& pattern_candidates, uint32_t k, uint32_t nevents, uint32_t min_gap, uint32_t max_gap);

			void count_candidates_with_brute_force(std::vector<Pattern>& pattern_candidates, uint32_t min_support, uint32_t min_gap, uint32_t max_gap);

			bool check_item_set(const item_set& items_parent, const item_set& items_child);

			void generate(Pattern& pattern, const Pattern& pattern_a, const Pattern& pattern_b);

			void generate_two(Pattern& newpattern, const Pattern& pattern_a, const Pattern& pattern_b);

			void generate_two(Pattern& newpattern_a, Pattern& newpattern_b, Pattern& newpattern_c, 
										const Pattern& pattern_a, const Pattern& pattern_b);
		private:
			std::vector<Pattern> patterns;	
			std::map<int, Sequence> sequences;
			uint32_t _m_max_sequences_length;
			typedef std::map<int, Sequence>::iterator sequences_iterator;
			typedef std::vector<Pattern>::iterator patterns_iterator;
			static DataBase *_m_instance;
			DataBase(){}
	};
}//namespace gsp
#endif // DATABASE_H
