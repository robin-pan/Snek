#ifndef __CELL_H__
#define __CELL_H__

#include "gridState.h"

class Cell {
	Gridstate _gridstate;

public:
	Cell();
	~Cell();

	Gridstate getState() const;
	void setState(Gridstate gridState);
};
#endif