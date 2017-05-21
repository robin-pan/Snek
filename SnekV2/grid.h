#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <utility>
#include <random>
#include "cell.h"
#include "snake.h"
#include "gridstates.h"


class Grid {
	std::vector<std::vector<Cell>> _theGrid;
	int _gridSize;
	std::pair<int, int> _foodLocation;
	Snake s;

public:
	Grid(int gridSize);

	void reset();
	void moveFood();
	std::pair<int, int> foodLocation();
	int gridSize();
	bool update();
	void changeHeadDirection(GridState direction);

	Cell& operator[](std::pair<int, int>& p) {
		return _theGrid[p.first][p.second];
	}

	friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif