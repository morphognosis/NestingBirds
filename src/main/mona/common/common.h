// Common header

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <list>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <algorithm>
using namespace std;
using std::string;

#include "fileio.h"
#include "gettime.h"
#include "log.hpp"
#include "matrix.hpp"
#include "md5.h"
#include "quaternion.hpp"
#include "random.hpp"
#include "vector.hpp"
#include "valueset.hpp"
#include "RDtree.hpp"

#define DONT_CARE      (-1)
#define NEARLY_ZERO    0.0001
