#include "stdafx.h"
#include "GPU.h"

GPU::GPU()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Polybius", 100, 100, 64 * 14, 32 * 20, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 0, 64, 32);
	memset(pixels, 0, sizeof(pixels));
}

void GPU::Render() {
	uint16_t a = 64 * 32;
	for (uint16_t i = 0; i < a; ++i) {
		pixels[i] = vram[i] * 0x00FFFFFF;
	}
	SDL_UpdateTexture(texture, NULL, pixels, 64 * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

GPU::~GPU()
{
}
