#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <utility>
#include <random>
#include "cell.h"
#include "snake.h"
#include "gridstates.h"
#include "oxygine-framework.h"
#include <functional>
#include "gdisplay.h"
using namespace oxygine;

const int _BLOCK_SIZE = 25;

class Grid {
	bool _isLengthOne;
	std::vector<std::vector<Cell>> _theGrid;
	std::pair<int, int> _foodLocation;
	Snake s;
	const int _GRID_SIZE;
	GraphicsDisplay _gd;

public:
	Grid();

	static Grid& instance();
	void reset();
	void moveFood();
	std::pair<int, int> foodLocation();
	int gridSize();
	bool update();
	void changeHeadDirection(GridState direction);
	GridState getHeadDirection();
	bool isLengthOne();

	Cell& operator[](std::pair<int, int>& p) {
		return _theGrid[p.first][p.second];
	}

	friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif