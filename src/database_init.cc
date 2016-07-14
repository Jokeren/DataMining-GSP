#include <algorithm>
#include "database.h"
#include "gsp.h"

namespace gsp {
	void DataBase::sort_sequences()
	{
		for (sequences_iterator it = sequences.begin(); it != sequences.end(); ++it) {
		//sort the sqeuence by timestamp
			Sequence& sequence = it->second;
			std::sort(sequence.begin(), sequence.end());
		//make item-time list
			sequence.make_list();
		}
	}

	void DataBase::push_item_set(const item_set& items, uint32_t sequence_id) 
	{
		//We assume that all the sequence id are greater than 0
		if (sequence_id >= 0) {
			sequences[sequence_id].push_back(items);
			if(sequences[sequence_id].get_sequence_id()!=sequence_id)
				sequences[sequence_id].set_sequence_id(sequence_id);
		} else {
			//To-do: report error!
		}
	}	

	void DataBase::print_patterns()
	{
		for (size_t i = 0; i < patterns.size(); ++i) {
			Pattern& pattern = patterns[i];
			printf("%s:%u\n", pattern.c_str(), pattern.get_support());
		}
	}

}//namespace gsp
