#include <algorithm>
#include "database.h"
#include "gsp.h"

namespace gsp {
	bool DataBase::count_pattern(const Pattern& pattern, uint32_t min_support, uint32_t min_gap, uint32_t max_gap, uint32_t& ncount) 
	{
		for (sequences_iterator it = sequences.begin(); it != sequences.end(); ++it) {
			if (check_pattern_sequence(pattern, it->second, min_gap, max_gap)) {
				++ncount;	
			}
		}

		if (ncount < min_support) {
			return false;
		} else {
			return true;
		}
	}


	bool DataBase::check_pattern(const Pattern& pattern_a, const Pattern& pattern_b) 
	{
		if (pattern_a.get_length() != pattern_b.get_length()) {
			//To-do: report error!
			exit(-1);
		}

		//check each item
		size_t idx_a = 0, idx_b = 0;
		uint32_t size_a = 0, size_b = 0;

		item_set::iterator it_a = pattern_a[idx_a].begin();
		item_set::iterator it_b = pattern_b[idx_b].begin();

		//ignore the first item_set
		++it_a;	
		if (it_a == pattern_a[idx_a].end()) {
			++idx_a;
			it_a = pattern_a[idx_a].begin();
		}

		//very tricky implementation
		while (true) {
			//check size of item_set after drop the first and last item
			if (idx_a == 0) {
				size_a = pattern_a[idx_a].size() - 1;	
			} else {
				size_a = pattern_a[idx_a].size();
			}

			if (idx_b == pattern_b.size() - 1) {
				size_b = pattern_b[idx_b].size() - 1;
			} else {
				size_b = pattern_b[idx_b].size();
			}

			if (size_a != size_b) {
				return false;
			}

			while (it_a != pattern_a[idx_a].end()) {
				if (it_a->id != it_b->id) {
					return false;
				}	
				++it_a;
				++it_b;
			}

			//forward a
			++idx_a;
			if (idx_a == pattern_a.size()) {
				break;
			} else {
				it_a = pattern_a[idx_a].begin();
			}

			//if b is the end, not forward
			if (idx_b != pattern_b.size() - 1) {
				++idx_b;
				it_b = pattern_b[idx_b].begin();
			}
		}

		return true;
	}

	//child must be in the parent
	bool DataBase::check_item_set(const item_set& items_parent, const item_set& items_child) 
	{
		item_set::iterator it_child = items_child.begin();
		item_set::iterator it_parent = items_parent.begin();

		while (it_child != items_child.end() && it_parent != items_parent.end()) {
			if (it_parent->id > it_child->id) {
				return false;
			} else if (it_parent->id == it_child->id) {
				++it_child;
				++it_parent;
			} else {
				++it_parent;
			}
		}

		if (it_parent == items_parent.end() && it_child != items_child.end()) 
			return false;
		else 
			return true;
	}

	bool DataBase::check_pattern_sequence(const Pattern& candidate, const Sequence& sequence, uint32_t min_gap, uint32_t max_gap)
	{
		std::vector<size_t> pos(candidate.size() + 1);

		size_t start_idx = 0;//candidate index
		size_t find_idx = 0;//sequence index
		while (true) {
			bool find = false;
			for (size_t i = find_idx; i < sequence.size(); ++i) {
				if (check_item_set(sequence[i], candidate[start_idx])) {
					if(start_idx > 0 && sequence[i].timestamp - sequence[pos[start_idx - 1]].timestamp < min_gap)
						continue;
					find = true;
					find_idx = i;
					break;
				}
			}
			if (!find) {
				return false;
			} else {
				if (start_idx == 0) {
					pos[start_idx] = find_idx;
					++start_idx;
					++find_idx;
				} else {
					if (sequence[find_idx].timestamp - 
							sequence[pos[start_idx - 1]].timestamp > max_gap) {
						uint32_t prev_timestamp = sequence[find_idx].timestamp - max_gap;
						for (size_t i = find_idx - 1; i >= 0; --i) {
							if (sequence[i].timestamp < prev_timestamp) {
								find_idx = i + 1;
								break;
							}
						}

						--start_idx;
					} else {
						pos[start_idx] = find_idx;
						++start_idx;
						++find_idx;
					}
				}
			}
			if (start_idx == candidate.size()) {
				return true;
			}
		}
	}

	bool DataBase::check_pattern_sequence_time_list(const Pattern& candidate, const Sequence& sequence, uint32_t min_gap, uint32_t max_gap)
	{
		std::map<uint32_t, size_t> pos;
		std::vector<int32_t> times(candidate.size() + 1);

		size_t start_idx = 0;

		while (true) {
			const item_set& items = candidate[start_idx];
			bool find_items = true;
			int32_t max_time = -1;

			item_set::iterator it_items = items.begin();

			while (it_items != items.end()) {
				int32_t tmp = pos[it_items->id];
				int32_t time = sequence.get_timestamp(it_items->id, pos[it_items->id]);

				//printf("max_time %d time %d\n", max_time, time);
				if (time == -1) {
					find_items = false;
					break;
				}

				if (max_time == -1) {
					max_time = time;
					++it_items;
				} else if (time > max_time) {
					max_time = time;
					it_items = items.begin();
				} else if (max_time > time) {
					int32_t next_time = 0;
					size_t idx = pos[it_items->id];

					while (next_time != -1) {
						next_time = sequence.get_timestamp(it_items->id, idx);
						if (next_time >= max_time) {
							pos[it_items->id] = idx;
							break;
						} 
						++idx;
					}

					if (next_time == -1) {
						find_items = false;
						break;
					} else if (next_time > max_time) {
						max_time = next_time;	
						it_items = items.begin();
					} else {
						++it_items;
						//do nothing
					}
				} else {
					//do nothing
					++it_items;
				}

				if (it_items == items.end()) {
					for (item_set::iterator it = items.begin(); it != items.end(); ++it) {
						++pos[it->id];
					}
					find_items = true;
					break;
				}
			}

			if (!find_items)
				return false;

			if (start_idx == 0) {
				if (max_time < max_gap) {
					times[start_idx] = max_time;
					++start_idx;
				} 
			} else {
				if (times[start_idx - 1] + max_gap <= max_time) {
					--start_idx;
				} else if (times[start_idx - 1] + min_gap < max_time) {
					times[start_idx] = max_time;
					++start_idx;
				}  else {//max_time - times[start_idx - 1] < min_gap
					//do nothing
				}
			}

			if (start_idx == candidate.size()) 
				return true;
		}
	}

}//namespace gsp
