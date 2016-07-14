#include <sstream>
#include "gsp.h"

namespace gsp {
	void Pattern::transform() 
	{
			//print numbers into the string
		std::stringstream ss;

		for (size_t i = 0; i < this->size(); ++i) {
			item_set& items = (*this)[i];
			ss << "(";
			for (item_set::iterator it = items.begin(); it != items.end(); ++it) {
				ss << it->id;

				item_set::iterator it_next = it;
				if (++it_next != items.end()) {
					ss << ",";
				}
			}
			ss << ")";
		}

		_m_pattern = ss.str();
	}
	
	const char* Pattern::c_str() 
	{
		//a tricky implementation, please do not use it, or notice it
		if (_m_pattern.size() != _m_length)
			transform();

		return _m_pattern.c_str();
	}


	void Pattern::transform_to_list()
	{
		_m_items.clear();
		_m_items.reserve(_m_length);
		for (size_t i = 0; i < this->size(); ++i) {
			item_set& items = (*this)[i];
			for (item_set::iterator it = items.begin(); it != items.end(); ++it) {
				_m_items.push_back(it->id);
			}
		}
	}

	const std::vector<uint32_t> Pattern::items_list() 
	{
		//a tricky implementation, please do not use it, or notice it
		if (_m_items.size() != _m_length)
			transform_to_list();

		return _m_items;
	}

}//namespace gsp
