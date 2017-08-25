#include <string>
#include <iostream>
#include "grid.h"
#include "snake.h"

Grid& Grid::instance()
{
	static Grid sGrid;

	return sGrid;
}

Grid::Grid() : _GRID_SIZE(int(getStage()->getSize().y / _BLOCK_SIZE)), _gd(_GRID_SIZE, _BLOCK_SIZE) {
	_theGrid.resize(_GRID_SIZE);

	for (auto& i : this->_theGrid) {
		i.resize(_GRID_SIZE);
	}

	int row = 0;
	for (auto& i : this->_theGrid) {
		int col = 0;
		for (auto& j : i) {
			j.registerObserver(_gd);

			auto newLoc = std::make_pair(row, col);
			j.setPosition(newLoc);
			col++;
		}
		row++;
	}

	this->reset();

	s = Snake();
}

void Grid::reset() {
	for (auto& i : this->_theGrid) {
		for (auto& j : i) {
			j.setState(GridState::NONE);
			j.notifyObservers();
		}
	}

	this->_theGrid[0][0].setState(GridState::RIGHT);
	this->_theGrid[0][0].notifyObservers();

	s.setHead(std::make_pair(0, 0));
	s.setTail(std::make_pair(0, 0));

	_isLengthOne = true;

	this->moveFood();
}

void Grid::moveFood() {
	std::uniform_int_distribution<> dis(0, (_GRID_SIZE - 1));
	std::random_device rd;
	std::mt19937 rand{ rd() };

	int row = 0;
	int column = 0;
	auto head = s.head();

	do {
		row = dis(rand);
		column = dis(rand);
	} while (_theGrid[row][column].getState() != GridState::NONE);

	_theGrid[row][column].setState(GridState::FOOD);
	_theGrid[row][column].notifyObservers();
	_foodLocation = std::make_pair(row, column);
}

int Grid::gridSize() {
	return _GRID_SIZE;
}

std::pair<int, int> Grid::foodLocation() {
	return _foodLocation;
}

bool Grid::update() {
	GridState oldHeadState = (*this)[s.head()].getState();

	auto oldHead = s.head();
	auto newHead = oldHead;
	auto oldTail = s.tail();
	auto newTail = oldTail;

	// Find next location of head
	switch (oldHeadState) {
	case GridState::DOWN:
		newHead = std::make_pair(oldHead.first + 1, oldHead.second);
		break;
	case GridState::UP:
		newHead = std::make_pair(oldHead.first - 1, oldHead.second);
		break;
	case GridState::LEFT:
		newHead = std::make_pair(oldHead.first, oldHead.second - 1);
		break;
	case GridState::RIGHT:
		newHead = std::make_pair(oldHead.first, oldHead.second + 1);
		break;
	}

	// Find next location of tail
	switch ((*this)[oldTail].getState()) {
	case GridState::DOWN:
		newTail = std::make_pair(oldTail.first + 1, oldTail.second);
		break;
	case GridState::UP:
		newTail = std::make_pair(oldTail.first - 1, oldTail.second);
		break;
	case GridState::LEFT:
		newTail = std::make_pair(oldTail.first, oldTail.second - 1);
		break;
	case GridState::RIGHT:
		newTail = std::make_pair(oldTail.first, oldTail.second + 1);
		break;
	}

	// Check if next location of head is out of bounds
	if (newHead.first >= (*this).gridSize() || newHead.second >= (*this).gridSize()) {
		return false;
	}

	if (newHead.first < 0 || newHead.second < 0) {
		return false;
	}

	// Check if new snake head is overlapping with body 
	GridState newState = (*this)[newHead].getState();
	if (newState != GridState::NONE && newState != GridState::FOOD) {
		return false;
	}

	// Update tail
	if (newHead != _foodLocation) {
		(*this)[oldTail].setState(GridState::NONE);
		(*this)[oldTail].notifyObservers();

		s.setTail(newTail);
	}
	else {
		if (_isLengthOne) _isLengthOne = false;
	}

	// Check if new snake head is overlapping with body again
	newState = (*this)[newHead].getState();
	if (newState != GridState::NONE && newState != GridState::FOOD) {
		return false;
	}


	// Update head
	s.setHead(newHead);
	(*this)[newHead].setState(oldHeadState);
	(*this)[newHead].notifyObservers();

	if (newHead == _foodLocation) {
		moveFood();
	}

	return true;
}

void Grid::changeHeadDirection(GridState direction) {
	(*this)[s.head()].setState(direction);
}

GridState Grid::getHeadDirection() {
	return _theGrid[s.head().first][s.head().second].getState();
}

bool Grid::isLengthOne() {
	return _isLengthOne;
}

std::ostream &operator<<(std::ostream &out, const Grid &g) {
	for (auto i : g._theGrid) {
		for (auto j : i) {
			out << std::to_string(int(j.getState())) << "\t";
		}
		out << std::endl;
	}

	return out;
}
