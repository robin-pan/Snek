#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <utility>
#include "gridstates.h"

class Snake {
	std::pair<int, int> _head;
	std::pair<int, int> _tail;

public:
	Snake();

	std::pair<int, int>& head();
	std::pair<int, int>& tail();

	void setHead(std::pair<int, int> newHead);
	void setTail(std::pair<int, int> newTail);
	
};
#endif