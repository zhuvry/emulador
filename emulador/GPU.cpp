#include "stdafx.h"
#include "GPU.h"

GPU::GPU()
{
	window = SDL_CreateWindow("Polybius", 100, 1000, 64 * 8, 32 * 12, 0);
	render = SDL_CreateRender(window, -1, SDL_RENDER_ACCELERATED);
	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, 0, 64, 32);
	memset(pixels, 0, sizeof(pixels));
}

void GPU::Render() {
	unsigned short a = 64 * 32;
	for (unsigned short i = 0; i < a; ++i) {
		pixels[i] = vram[i] * 0xFFFFFFFF;
	}
	SDL_UpdateTexture(texture, NULL, pixels, 64 * 64);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresente(renderer)
}

GPU::~GPU()
{
}
