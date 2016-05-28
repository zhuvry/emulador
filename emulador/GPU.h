#pragma once
#include <SDL.h>ñ

class GPU
{
private:
	SDL_Window* window;
	SDL_Render* renderer;
	SDL_Texture* texture;
	Uint pixels[64 * 32];
public:
	int vram[64 * 32];
	GPU();
	void Render();
	~GPU();
};

