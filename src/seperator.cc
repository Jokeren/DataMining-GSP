#include "seperator.h"

static const int MAX_LENGTH = 10240;

namespace gsp {
	uint32_t Seperator::seperate(const char* file_path, 
			std::vector<std::vector<std::string> > & stream) {
		char line[MAX_LENGTH];

		FILE *fp = fopen(file_path, "r+");

		if (fp == NULL) {
			fprintf(stderr, "error occurs when reading file %s\n", file_path);
			return -1;
		}
		
		uint32_t ncount = 0;

		while (fgets(line, MAX_LENGTH - 1, fp) != NULL) {
			stream.resize(ncount + 1);
			char *pch = NULL;
			pch = strtok(line, _m_token);
			while (pch != NULL) {
				stream[ncount].push_back(std::string(pch));
				pch = strtok(NULL, _m_token);
			}
			ncount++;
		}	

		return ncount;
	};
}//namespace gsp

