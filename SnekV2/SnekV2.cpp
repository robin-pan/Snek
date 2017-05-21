#include "stdafx.h"
#include "grid.h"
#include "snake.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[]) {
	Grid g(5);
	char i = 0;

	while (true) {
		std::cout << g;
		std::cin >> i;

		if (i == 's') {
			g.changeHeadDirection(GridState::DOWN);
		}
		else if (i == 'w') {
			g.changeHeadDirection(GridState::UP);
		}
		else if (i == 'a') {
			g.changeHeadDirection(GridState::LEFT);
		}
		else if (i == 'd') {
			g.changeHeadDirection(GridState::RIGHT);
		}
		g.update();
	}

	return 0;
}