#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"

int main()
{	
	Game game(960, 720);
	game.Run();

	return 0;
}
