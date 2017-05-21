#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>

int main() {

	return 0;
}

/*
#include <string>
#include <sstream>
#include <cmath>
#include <stdlib.h>     
#include <time.h>      
#include <cassert>
#include <cstdlib>
#include <type_traits>
#include <iostream>
#include <array>
#include <utility>

using std::move;
using std::array;

//The numbers assigned to the grid of the game
enum Gridstate
{
	TILE_WEST = -2,
	TILE_SOUTH,
	TILE_NONE,
	TILE_NORTH,
	TILE_EAST,
	TILE_FOOD
};

#define IS_DIRECTION(dir) (dir && (dir >= TILE_WEST && dir <= TILE_EAST))
#define ARE_OPPOSITE(dir1, dir2) (!(dir1 + dir2))

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Game dimensions
const int GRID_WIDTH = SCREEN_HEIGHT / 10;
const int GRID_HEIGHT = SCREEN_HEIGHT / 10;

//Grid
array<array<int, GRID_HEIGHT>, GRID_WIDTH> grid;

//FPS of game
int FPS = 15;

//Score and lives counter
int score = 0;
int lives = 3;

//FPS Counter
Uint32 start = 0;

//Texture wrapper class which was borrowed from the internet (credits to Lazy Foo Productions)
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Text font
TTF_Font *gFont = NULL;

//Title screen stuff
SDL_Surface *titleSurface = nullptr;
SDL_Surface *windowSurface = nullptr;

//Rendered texture
LTexture gTextTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

int randomcoord()
{
	return (rand() % (GRID_HEIGHT));
}

//Coordinates of head, tail and food
static SDL_Point head{ 0, 0 };
static SDL_Point tail{ 0, 0 };
static SDL_Point food{ 0, 0 };

void newfoodpos()
{
	do
	{
		food.x = randomcoord();
		food.y = randomcoord();
	} while (grid[food.x][food.y]);
}

static bool operator ==(const SDL_Point& p1, const SDL_Point& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

SDL_Point offset_of(int state)
{
	SDL_Point offset{ 0, 0 };
	switch (state)
	{
	case TILE_EAST:
		++offset.x;
		break;
	case TILE_WEST:
		--offset.x;
		break;
	case TILE_NORTH:
		--offset.y;
		break;
	case TILE_SOUTH:
		++offset.y;
		break;
	default:
		assert(!"'state' does not denote a direction");
	}
	return move(offset);
}

void updatepos(SDL_Point& point, const SDL_Point& offset)
{
	point.x += offset.x;
	point.y += offset.y;
}

bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	gWindow = SDL_CreateWindow("Snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool loadMedia()
{
	//Open the font
	gFont = TTF_OpenFont("Times.ttf", 14);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	return true;

}

void close()
{
	//Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_FreeSurface(titleSurface);

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//Clears the board, resets head and tail location, and sets the direction to east
void setGrid() {
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			grid[i][j] = TILE_NONE;
		}
	}
	head.x = 0;
	head.y = 0;
	tail.x = 0;
	tail.y = 0;

	newfoodpos();

	grid[head.x][head.y] = TILE_EAST;
	grid[food.x][food.y] = TILE_FOOD;
}

//Contains the game loop
void game() 
{
	//Rectangles which will represesnt the food and the snake
	SDL_Rect foodRect, snekSegment;

	setGrid();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Start timer
		start = SDL_GetTicks();

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (!ARE_OPPOSITE(grid[head.x][head.y], TILE_NORTH)) grid[head.x][head.y] = TILE_NORTH;
					break;
				case SDLK_DOWN:
					if (!ARE_OPPOSITE(grid[head.x][head.y], TILE_SOUTH)) grid[head.x][head.y] = TILE_SOUTH;
					break;
				case SDLK_LEFT:
					if (!ARE_OPPOSITE(grid[head.x][head.y], TILE_WEST)) grid[head.x][head.y] = TILE_WEST;
					break;
				case SDLK_RIGHT:
					if (!ARE_OPPOSITE(grid[head.x][head.y], TILE_EAST)) grid[head.x][head.y] = TILE_EAST;
					break;
				case SDLK_c:
					setGrid();
					break;
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render blue filled rect to highlight playing field 
		SDL_Rect fillRect = { 0, 0, GRID_WIDTH * 10, GRID_HEIGHT * 10 };
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);

		//Render black rectangles to represent food
		foodRect = { food.x * 10, food.y * 10, 10, 10 };
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &foodRect);

		//Draw in green rectangles wherever snake segments occupy grid
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		for (int i = 0; i < GRID_WIDTH; i++)
		{
			for (int j = 0; j < GRID_HEIGHT; j++)
			{
				if (IS_DIRECTION(grid[i][j]))
				{
					snekSegment = { i * 10, j * 10, 10, 10 };
					SDL_RenderFillRect(gRenderer, &snekSegment);
				}
			}
		}

		//Sends the data to be converted into strings
		std::stringstream scorestream;
		std::stringstream livestream;
		scorestream << score;
		livestream << lives;

		SDL_Color textColor = { 0, 0, 0 };

		//Render text in the form of rectangles
		gTextTexture.loadFromRenderedText("Lives " + livestream.str(), textColor);
		gTextTexture.render(490, 35);

		gTextTexture.loadFromRenderedText("Score: " + scorestream.str(), textColor);
		gTextTexture.render(490, 50);

		//Update screen
		SDL_RenderPresent(gRenderer);

		//Maintain FPS
		if (1000 / FPS > SDL_GetTicks() - start)
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));

		int current_direction = grid[head.x][head.y];
		SDL_Point offset = offset_of(current_direction);

		updatepos(head, offset);

		// if head gets to the edge
		if (head.x < 0 || head.y < 0 || head.x > GRID_WIDTH || head.y > GRID_WIDTH) {
			lives--;
			if (lives < 0) {
				break;
			}
			else {
				setGrid();
				continue;
			}
		}

		//This indicates that the snake has bitten itself
		if (IS_DIRECTION(grid[head.x][head.y]))
		{
			//Subtract one life 
			lives--;

			//Reset FPS to 15
			FPS = 15;

			if (lives < 0) {
				//Game over if player runs out of lives
				break;
			}
			else if (lives >= 0)
			{
				setGrid();
				continue;
			}
		}

		//This indicates that the food has been eaten
		if (head == food)
		{
			//Respawn the food
			newfoodpos();

			//Increment score
			score += 1;
			FPS += 1;
		}
		else
		{
			//Snake keeps moving
			SDL_Point tail_offset = offset_of(grid[tail.x][tail.y]);
			grid[tail.x][tail.y] = TILE_NONE;
			updatepos(tail, tail_offset);
		}

		grid[head.x][head.y] = current_direction;
	}
}

int main(int argc, char* args[])
{
	//Main loop flag
	bool quit = false;

	SDL_Event e;
	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}

	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
	}

	//Start timer
	start = SDL_GetTicks();

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{
				game();
				quit = true;
				break;
			}

			SDL_BlitSurface(titleSurface, NULL, windowSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);

		}
	}
	//Free resources and close SDL
	close();

	return 0;
}
*/