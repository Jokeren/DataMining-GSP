#include "gsp.h"

namespace gsp {
	void Sequence::make_list()
	{
		for	(size_t i = 0; i < size(); ++i) {
			item_set& items = (*this)[i];
			uint32_t timestamp = items.timestamp;

			for (item_set::iterator it = items.begin(); it != items.end(); ++it) {
				_m_items_time_list[it->id].push_back(timestamp);
			}
		}	

//		for (std::map<uint32_t, std::vector<uint32_t> >::iterator it = 
//				_m_items_time_list.begin(); it != _m_items_time_list.end(); ++it) {
//			printf("%u : ", it->first);	
//			for (size_t i = 0; i < (it->second).size() - 1; ++i) {
//				printf("%u->", (it->second)[i]);
//			}
//			printf("%u\n", (it->second)[it->second.size() - 1]);
//		}
//		printf("\n");
	}

	int32_t Sequence::get_timestamp(uint32_t id, uint32_t idx) const 
	{
		std::map<uint32_t, std::vector<uint32_t> >::const_iterator it = _m_items_time_list.find(id);
		if (it == _m_items_time_list.end() || idx >= (it->second).size()) {
			return -1;
		}

		return (it->second)[idx];
	}
}//namespace gsp
