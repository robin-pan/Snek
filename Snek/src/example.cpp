#include "oxygine-framework.h"
#include <functional>
#include "grid.h"
#include "example.h"

using namespace oxygine;

int delay = 1;

void example_preinit() 
{
}

//called from main.cpp
void example_init()
{
	
}


//called each frame from main.cpp
void example_update()
{
	if (delay % 4 == 0) {
		Grid& g = Grid::instance();

		const Uint8* data = SDL_GetKeyboardState(nullptr);

		GridState newGridState = g.getHeadDirection();

		if (data[SDL_GetScancodeFromKey(SDLK_a)] || data[SDL_GetScancodeFromKey(SDLK_LEFT)]) {
			newGridState = GridState::LEFT;
		}
		else if (data[SDL_GetScancodeFromKey(SDLK_d)] || data[SDL_GetScancodeFromKey(SDLK_RIGHT)]) {
			newGridState = GridState::RIGHT;
		}
		else if (data[SDL_GetScancodeFromKey(SDLK_w)] || data[SDL_GetScancodeFromKey(SDLK_UP)]) {
			newGridState = GridState::UP;
		}
		else if (data[SDL_GetScancodeFromKey(SDLK_s)] || data[SDL_GetScancodeFromKey(SDLK_DOWN)]) {
			newGridState = GridState::DOWN;
		}

		if (int(newGridState) == -1 * int(g.getHeadDirection()) && !g.isLengthOne()) {
			g.update();
		}
		else {
			g.changeHeadDirection(newGridState);

			if (!g.update()) {
				g.reset();
			}
		}
		

		delay = 1;
	}
	else {
		delay++;
	}
}

//called each frame from main.cpp
void example_destroy()
{
}
