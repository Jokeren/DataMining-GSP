#include <cstring>
#include "gsp.h"
#include "gsp_utils.h"
#include "database.h"
#include "common.h"
//N_ARG and ARGH[N_ARG] hava been moved to gsp_utils.h as static variables
//Notice: do not move this!!!
//generate singleton database
using gsp::DataBase;
DataBase* DataBase::_m_instance = new DataBase();

int main(int argc, char* argv[])
{
	uint32_t n_argh = 0;
	uint32_t nsupport = 0;

	gsp::parameters in_parameters;
	n_argh = gsp::get_parameters(&in_parameters, argc, argv);

	if(n_argh != gsp::get_narg() - 1)
	{
		gsp::usage();
		exit(-1);
	}

	std::string seperator;
	gsp::GspFileType file_type;
	gsp::get_seperator(&in_parameters, seperator);
	gsp::get_file_type(&in_parameters, file_type);
	gsp::print_parameters(&in_parameters);

	//uint32_t get_nsupport(gsp::parameters* p_parameters);
	nsupport = gsp::get_nsupport(&in_parameters);
	printf("nsupport %u\n", nsupport);

	if (nsupport == 0) {
		gsp::usage();
		exit(-1);
	}

	//confirm parameters
	gsp::print_parameters(&in_parameters);

	//freopen("temp2.out", "w", stdout);
	gsp::Gsp gsp;
	gsp.execute(in_parameters.file_path, nsupport, in_parameters.nsequences, in_parameters.nevents, 
			in_parameters.min_gap, in_parameters.max_gap, seperator.c_str(), file_type);


	return 0;
}
