#include "grid.h"
#include "gridState.h"

Grid::Grid() {
	_theGrid.resize(_gridSize);
	
	for (auto it : _theGrid) {
		it.resize(_gridSize);
	}

	this->reset();
}

Grid::~Grid() {

}

void Grid::reset() {
	for (auto it : _theGrid) {
		for (auto itt : it) {
			itt.setState(Gridstate::NONE);
		}
	}

	_theGrid[0][0].setState(Gridstate::EAST);
}