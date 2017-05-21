#include "cell.h"

Cell::Cell() : _gridstate(Gridstate::NONE) {

}

Cell::~Cell() {

}

Gridstate Cell::getState() const {
	return _gridstate;
}

void Cell::setState(Gridstate newGridState) {
	_gridstate = newGridState;
}