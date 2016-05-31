#ifndef _SYSGETOPT_H_
#define _SYSGETOPT_H_

#include "prjconf.h"

#if defined (PRJCONF_WINDOWS)
#if defined(_MSC_VER) && _MSC_VER < 1900

// windows portion of getopt is taken from:
// https://gist.github.com/superwills/5815344

extern char *optarg;

int getopt(int nargc, char * const nargv[], const char *ostr);

#endif // defined(_MSC_VER) && _MSC_VER < 1900
#else
#include <getopt.h>
#endif // defined (PRJCONF_WINDOWS)

#endif
