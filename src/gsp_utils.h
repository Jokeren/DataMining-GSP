#ifndef GSP_UTILS_H
#define GSP_UTILS_H

#include "common.h"

namespace gsp {
	const static uint32_t N_ARG = 8;
	const static char *ARGH[N_ARG] = { "-i", "-t", "-sequNUM", "-eventNUM", "-min", "-max", "-sep", "-file_type"};

	//used to save te parameters in the function of get_parameters;
	struct parameters{
		char* file_path;
		double support;
		uint32_t nsupport;
		uint32_t nsequences;
		uint32_t nevents;
		uint32_t min_gap;
		uint32_t sep_type;
		uint32_t max_gap;
		uint32_t file_type;
	};

	//To-do: move to utils
	enum {
		DEFAULT,
		SEP_COUNT
	};

	uint32_t get_narg();
	double inline atof_(const char *str);
	uint32_t get_parameters(struct parameters* p_parameters, int argc, char* argv[]);
	// print the imformation of parameters, used in debug.
	void print_parameters(struct parameters* p_parameters);
	void get_seperator(struct parameters* p_parameters, std::string& seperator);
	void get_file_type(struct parameters* p_parameters, GspFileType& file_type);
	uint32_t get_nsupport(struct parameters* p_parameters);
	void usage();

}//namespace gsp
#endif
