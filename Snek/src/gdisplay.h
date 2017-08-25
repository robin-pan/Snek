#pragma once

#include <vector>
#include "observer.h"
#include "oxygine-framework.h"
#include <functional>
#include "gridstates.h"
using namespace oxygine;
typedef oxygine::intrusive_ptr<ColorRectSprite> spCRS;

class GraphicsDisplay : public Observer {
private:
	std::vector<std::vector<spCRS>> _g;
	const int _GRID_SIZE;
	const int _BLOCK_SIZE;
public:
	GraphicsDisplay(int gridSize, int blockSize);

	void changeColor(std::pair<int, int> &target, GridState& targetState);
	void updateFood(std::pair<int, int> &newLoc);
	void notify(Subject& whoNotified);
};