#include <vector>
#include "cell.h"

#ifndef __GRID_H__
#define __GRID_H__
class Grid {
	// 2d array of cells which represent the grid of the game
	std::vector<std::vector<Cell>> _theGrid;
	int _gridSize;

public:
	Grid();
	~Grid();

	void init();
	void reset();
};
#endif