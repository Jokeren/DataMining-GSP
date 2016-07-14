#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <string>

namespace gsp {

typedef std::vector<std::vector<std::string> > input;

typedef enum _Gsp_Return_Code {
	GSP_SUCCESS,
	GSP_WARNING,
	GSP_ERROR,
	GSP_RETURN_NCOUNT
} GspReturnCode;

typedef enum _Gsp_File_Type {
	GSP_NORMAL,
	GSP_SPMF,
	GSP_FILE_NCOUNT
} GspFileType;

}//namespace gsp
#endif
