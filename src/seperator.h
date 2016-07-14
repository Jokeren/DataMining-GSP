#ifndef SEPERATOR_H
#define SEPERATOR_H

#include "common.h"

namespace gsp {

class Seperator {
	public:
		Seperator(const char* token): _m_token(token) {
		};

		uint32_t seperate(const char *file_path, input& stream);

	private:
		const char* _m_token;	
};

}//namespace Gsp

#endif //SEPERATOR_H
