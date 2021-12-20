#ifndef _SYSGETOPT_H_
#define _SYSGETOPT_H_

#include "prjconf.h"

#if defined (PRJCONF_WINDOWS)

#ifdef __cplusplus
extern "C" {
#endif

// check if getopt is available in newer window versions
// defined(_MSC_VER) && _MSC_VER < 1900
// windows portion of getopt is taken from:
// https://gist.github.com/superwills/5815344

extern char *optarg;

int getopt(int nargc, char *const nargv[], const char *ostr);

#ifdef __cplusplus
}
#endif

#else

#include <getopt.h>

#endif

#endif
