#pragma once
#include <SDL.h>

class GPU
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	unsigned int pixels[64 * 32];
public:
	unsigned int vram[64 * 32];
	GPU();
	void Render();
	~GPU();
};

