#include <ctime>
#include "seperator.h"
#include "gsp.h"

namespace gsp{

void Gsp::execute(const char* file_path, uint32_t support, uint32_t nsequences,
		uint32_t nevents, uint32_t min_gap, uint32_t max_gap, const char *seperator_type,
		GspFileType file_type) {
	_m_support = support;
	_m_min_gap = min_gap;
	_m_max_gap = max_gap;
	_m_nevents = nevents;
	_m_nsequences = nsequences;
	_m_file_path = file_path;
	_m_seperator_type = seperator_type;
	_m_seperator = new Seperator(seperator_type);

	input stream;

	clock_t start, mid, finish, first_term;
	start = clock();

	_m_seperator->seperate(_m_file_path, stream);

	//read corresponding files
	if (file_type == GSP_NORMAL) {
		read_input_normal(stream);
	} else if (file_type == GSP_SPMF) {
		read_input_spmf(stream);
	}

	mid = clock();

	uint32_t k = 1;
	bool has_next = true;
	while (has_next) {
		has_next = generate_candidates(k);
		if (has_next) {
			print_patterns(k);
		}
		if (k == 1) {
			first_term = clock();	
		}
		++k;
	}

	finish = clock();
	printf("read : %f\ncomputing : %f\n first term : %f\n other_term: %f\n",
			(double)(mid-start)/CLOCKS_PER_SEC, (double)(finish-mid)/CLOCKS_PER_SEC, 
			(double)(first_term - mid)/CLOCKS_PER_SEC, (double)(finish - first_term)/CLOCKS_PER_SEC);
}

}//namespace gsp
