#ifndef __CELL_H__
#define __CELL_H__

#include <utility>
#include "gridstates.h"

class Cell {
	GridState _state;
	std::pair<int, int> _position;

public:
	Cell();

	GridState getState() const;
	void setState(GridState state);
};
#endif