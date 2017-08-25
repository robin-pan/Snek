#ifndef __INFO_H__
#define __INFO_H__

#include "gridstates.h"

struct Info {
	std::pair<int, int> location;
	GridState newState;
};

#endif
