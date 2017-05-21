#include "cell.h"

Cell::Cell() : _state(GridState::NONE) {

}

GridState Cell::getState() const {
	return _state;
}

void Cell::setState(GridState state) {
	_state = state;
}