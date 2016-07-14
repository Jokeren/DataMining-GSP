#include "gsp.h"
#include "database.h"

namespace gsp {
	bool Gsp::generate_candidates(uint32_t k)
	{
		DataBase *database = DataBase::get_instance();
		bool has_next = false;

		if (k == 1) {
			//1st iteration
			has_next = database->find_frequent_item_set(_m_nevents, _m_support);
		} else {
			//2nd iteration
			//kth iteration
			has_next = database->create_patterns(k, _m_nevents, _m_support, _m_min_gap, _m_max_gap);
		}
		
		return has_next;
	}
}//namespace gsp
