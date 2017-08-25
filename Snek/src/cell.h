#ifndef __CELL_H__
#define __CELL_H__

#include <utility>
#include "gridstates.h"
#include "subject.h"

class Cell : public Subject {
	GridState _state;
	std::pair<int, int> _position;
public:
	Cell();

	GridState getState() const;
	void setState(GridState state);
	void setPosition(std::pair<int, int>& newPos);
	std::pair<int, int> getPosition();
	Info getInfo();
};
#endif