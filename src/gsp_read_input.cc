#include <algorithm>
#include "gsp.h"
#include "database.h"

namespace gsp {

void Gsp::read_input_normal(const input& stream)
{
	DataBase *database = DataBase::get_instance(); 

	uint32_t sequence_id = 0;
	uint32_t timestamp = 0;
	uint32_t event_num = 0;
	uint32_t event_id = 0;
	uint32_t *items_time = new uint32_t[_m_nsequences+2]();

	for (size_t i = 0; i < stream.size(); ++i) {
		item_set items;
		
		for (size_t j = 0; j < stream[i].size(); ++j) {
			if (j == 0) {
				sscanf(stream[i][j].c_str(), "%u", &sequence_id);
			} else if (j == 1) {
				sscanf(stream[i][j].c_str(), "%u", &event_num);	
			} else {
				sscanf(stream[i][j].c_str(), "%u", &event_id);

				struct item_t item;
				item.id = event_id;
				items.insert(item);
			}
		}

		items.timestamp = items_time[sequence_id];
		items_time[sequence_id] += 1;
		database->push_item_set(items, sequence_id);
	}
	delete [] items_time;

	database->sort_sequences();
}

void Gsp::read_input_spmf(const input& stream)
{
	DataBase *database = DataBase::get_instance(); 

	uint32_t event_id = 0;
	uint32_t max_length = 0;

	for (size_t i = 0; i < stream.size(); ++i) {
		uint32_t timestamp = 0;
		uint32_t seq_length = 0;
		item_set items;
		
		int32_t buffer = 0;
		for (size_t j = 0; j < stream[i].size(); ++j) {
			sscanf(stream[i][j].c_str(), "%d", &buffer);

			if (buffer == -1) {
				items.timestamp = timestamp;
				++timestamp;
				++seq_length;

				database->push_item_set(items, i);
				items.clear();
				//printf(" time %u ", timestamp);
			} else if (buffer == -2) {
				break;
			} else {
				//printf("%d ", buffer);
				struct item_t item;
				item.id = buffer;
				items.insert(item);
			}
		}

		max_length = std::max(seq_length, max_length);
		//printf("\n");
	}

	database->set_max_sequence_length(max_length);
	database->sort_sequences();
}

}//namespace gsp
