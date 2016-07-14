#include <cmath>
#include "gsp_utils.h"

namespace gsp {

	//To-do: move to utils
	//To-do: write usage information
	//
	void usage()
	{
		printf("usage!\n");
	}

	//To-do: move to utils
	double inline atof_(const char *str)
	{
		size_t str_len = strlen(str);

		if(str[str_len-1] != '%')
		{
			return atof(str);
		}

		return atof(str) / 100.0;
	}

	//To-do: move to utils
	//To-do: integreate into a function
	uint32_t get_parameters(gsp::parameters* p_parameters, int argc, char* argv[])
	{
		char *file_path = NULL;
		uint32_t file_type;
		double support = 0.0f;
		uint32_t nsequences = 0;
		uint32_t nevents = 0;
		uint32_t min_gap = 0;
		uint32_t sep_type = 0;
		uint32_t max_gap = 0x7fffffff;
		uint32_t n_argh = 0;
		for (size_t i_argv = 1; i_argv < argc - 1; i_argv += 2)
		{
			for (size_t i_argh = 0; i_argh < N_ARG; i_argh++)
			{                                               
				if (strcmp(argv[i_argv], ARGH[i_argh]) != 0)
				{
					continue;
				} 
				switch (i_argh)
				{
					case 0: file_path = argv[i_argv + 1];
							n_argh++;
							break;
					case 1: support = atof_(argv[i_argv + 1]);
							if(support > 1 || support <= 0)
							{
								break;
							}
							n_argh++;
							break;
					case 2: nsequences = atoi(argv[i_argv + 1]);
							if(nsequences <= 0)
							{
								break;
							}
							n_argh++;
							break;
					case 3: nevents = atoi(argv[i_argv + 1]);
							if(nevents <= 0)
							{
								break;
							}
							n_argh++;
							break;
					case 4: min_gap = atoi(argv[i_argv + 1]);
							if(min_gap < 0)
							{
								break;
							}
							n_argh++;
							break;
					case 5: max_gap = atoi(argv[i_argv + 1]);
							if(max_gap < 0)
							{
								break;
							}
							n_argh++;
							break;
					case 6: sep_type = atoi(argv[i_argv + 1]);
							if (sep_type < 0 || sep_type > SEP_COUNT) {
								usage();
								exit(-1);
							}
							break;
					case 7: file_type = atoi(argv[i_argv + 1]);
							if (sep_type < 0 || sep_type > GSP_FILE_NCOUNT) {
								usage();
								exit(-1);
							}
							n_argh++;
							break;
					default:
							usage();
							exit(-1);
				}
				break;
			}
		}
		p_parameters->file_path = file_path;
		p_parameters->file_type = file_type;
		p_parameters->support = support;
		//p_parameters->nsupport = nsupport;
		p_parameters->nsequences = nsequences;
		p_parameters->nevents = nevents;
		p_parameters->min_gap = min_gap;
		p_parameters->sep_type = sep_type;
		p_parameters->max_gap = max_gap;
		return n_argh;
	}

	void get_seperator(gsp::parameters* p_parameters,std::string &seperator)
	{
		if (p_parameters->sep_type == 0) {
			//To-do: seperator factory
			seperator = " ";
		} else {
		}
	}

	void get_file_type(struct parameters* p_parameters, GspFileType& file_type)
	{
		if (p_parameters->file_type == 0) {
			file_type = GSP_NORMAL;
		} else if (p_parameters->file_type == 1) {
			file_type = GSP_SPMF;
		} else {
			//do nothing
		}
	}

	uint32_t get_nsupport(gsp::parameters* p_parameters)
	{
		//dreturn static_cast<uint32_t>(p_parameters->support * p_parameters->nsequences + 0.5);
		return static_cast<uint32_t>(ceil(p_parameters->support * p_parameters->nsequences));
	}

	// print the imformation of parameters, used in debug.
	void print_parameters(gsp::parameters* p_parameters)
	{
		printf("-i: %s\n", p_parameters->file_path);
		printf("-t: %f\n", p_parameters->support);
		printf("-sequNUM: %u\n", p_parameters->nsequences);
		printf("-eventNUM: %u\n", p_parameters->nevents);
		printf("-min: %u\n", p_parameters->min_gap);
		printf("-max: %u\n", p_parameters->max_gap);
		return ;
	}

	uint32_t get_narg()
	{
		return N_ARG;
	}
}//namespace gsp
