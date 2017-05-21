#include "snake.h"

Snake::Snake() : _head(std::make_pair(0, 0)), _tail(std::make_pair(0, 0)) {
}

std::pair<int, int> Snake::head() {
	return _head;
}
std::pair<int, int> Snake::tail() {
	return _tail;
}

void Snake::setHead(std::pair<int, int> newHead) {
	_head = newHead;
}

void Snake::setTail(std::pair<int, int> newTail) {
	_tail = newTail;
}