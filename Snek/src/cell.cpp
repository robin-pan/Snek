#include "cell.h"
#include "info.h"
Cell::Cell() : _state(GridState::NONE) {

}

GridState Cell::getState() const {
	return _state;
}

void Cell::setPosition(std::pair<int, int>& newPos) {
	_position = newPos;
}

void Cell::setState(GridState state) {
	_state = state;
}

std::pair<int, int> Cell::getPosition() {
	return _position;
}

Info Cell::getInfo() {
	struct Info info = { _position, _state };
	return info;
}