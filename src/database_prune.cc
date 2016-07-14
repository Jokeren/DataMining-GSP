#include "database.h"
#include "gsp.h"

namespace gsp {
	bool DataBase::prune(const Pattern& pattern) 
	{
		for (size_t i = 0; i < pattern.size(); ++i) {
			if (pattern[i].size() > 1) {
				for (size_t j = 0; j < pattern[i].size(); ++j) {
					Pattern contiguous_pattern = pattern;
					item_set::iterator it = contiguous_pattern[i].begin();
					
					size_t k = 0;
					while (k < j) {
						++it;
						++k;
					}

					contiguous_pattern[i].erase(it);

					bool find = false;
					for (size_t i = 0; i < patterns.size(); ++i) {
						if (match_pattern(contiguous_pattern, patterns[i])) {
							find = true;
							break;
						}
					}

					if (!find) {
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool DataBase::match_item_set(const item_set& items_a, const item_set& items_b) 
	{
		if (items_a.size() != items_b.size()) {
			return false;
		}

		item_set::iterator it_a;
		item_set::iterator it_b;
		for (it_a = items_a.begin(), it_b = items_b.begin(); 
				it_a != items_a.end(); ++it_a, ++it_b) {
			if (it_a->id != it_b->id) {
				return false;
			}
		}

		return true;
	}

	bool DataBase::match_pattern(const Pattern& pattern_a, const Pattern& pattern_b) {
		if (pattern_a.size() != pattern_b.size()) {
			return false;
		}
		
		for (size_t i = 0; i < pattern_a.size(); ++i) {
			const item_set& items_a = pattern_a[i];
			const item_set& items_b = pattern_b[i];
			if (!match_item_set(items_a, items_b)) {
				return false;
			}
		}

		return true;
	}
}//namespace gsp
