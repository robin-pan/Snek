#include "gdisplay.h"
#include "subject.h"
#include "info.h"

class Subject;

GraphicsDisplay::GraphicsDisplay(int gridSize, int blockSize) : _GRID_SIZE(gridSize), _BLOCK_SIZE(blockSize) {
	_g.resize(_GRID_SIZE);

	int rloc = 0;
	for (auto &row : _g) {
		row.resize(_GRID_SIZE);

		int cloc = 160;
		for (auto &col : row) {
			col = new ColorRectSprite();
			col->setPosition(cloc, rloc);
			col->setSize(25, 25);
			col->attachTo(getStage());

			cloc += _BLOCK_SIZE;
		}

		rloc += _BLOCK_SIZE;
	}
}

void GraphicsDisplay::changeColor(std::pair<int, int> &target, GridState& targetState) {
	spCRS targetCell = _g[target.first][target.second];

	Color newColor = (targetState == GridState::NONE) ? Color::White : Color::Black;
	targetCell->setColor(newColor);
}

void GraphicsDisplay::updateFood(std::pair<int, int>& newLoc) {
	_g[newLoc.first][newLoc.second]->setColor(Color::Red);
	
}

void GraphicsDisplay::notify(Subject& whoNotified) {
	struct Info info = whoNotified.getInfo();

	if (info.newState == GridState::FOOD) {
		updateFood(info.location);
	}
	else {
		changeColor(info.location, info.newState);
	}
}