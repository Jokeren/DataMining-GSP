#include "gsp.h"
#include "database.h"

namespace gsp {
	void Gsp::print_patterns(uint32_t k)
	{
		DataBase *database = DataBase::get_instance();
		
		//printf("k = %u\tPatterns\t Support\n", k);

		database->print_patterns();
	}
}//namespace gsp
