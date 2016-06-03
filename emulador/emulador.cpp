// emulador.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "CPU.h"
#include <iostream>
#include <map>
#include "SDL.h"
#include <time.h>

using namespace std;
int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	SDL_Event event;
	CPU Polybius;
	std::map<int, int> MapKeys = { { SDLK_KP_0, 0 },{ SDLK_KP_1, 1 },{ SDLK_KP_2, 2 },{ SDLK_KP_3, 3 },{ SDLK_KP_4, 4 },
								   { SDLK_KP_5, 5 },{ SDLK_KP_6, 6 },{ SDLK_KP_7, 7 },{ SDLK_KP_8, 8 },{ SDLK_KP_9, 9 },
								   { SDLK_KP_A, 10 },{ SDLK_KP_B, 11 },{ SDLK_KP_C, 12 },{ SDLK_KP_D, 13 },{ SDLK_KP_E, 14 },{ SDLK_KP_F, 15 } };

	Polybius.loadROM("INVADERS");
	
	

	while (true)
	{
		// Emular la CPU
		Polybius.execute();
		// Si hay algún evento por teclado
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				// Si el evento es dejar de presionar una tecla
			case SDL_KEYUP:
				// Mira si es una tecla perteneciente al mapa de teclas
				// event.key.keysym.sym   ---->    Esto sirve para traer el valor de SDL de la tecla presionada
				if (MapKeys.count(event.key.keysym.sym))
					// Pone en 0 la tecla que ha dejado de ser presionada en la CPU
					Polybius.keys[MapKeys[event.key.keysym.sym]] = 0;
				break;


				// Si el evento es una tecla presionada
			case SDL_KEYDOWN:
				// Deben mirar si la tecla presionada es una tecla perteneciente al mapa de teclas
				// Poner en 1 la tecla presionada dentro de la CPU
				// Manejar el caso en el que se esté esperando una tecla WAITINGKEY
				if (MapKeys.count(event.key.keysym.sym)) {
					Polybius.keys[MapKeys[event.key.keysym.sym]] = 1;
					Polybius.waitingKey = MapKeys[(event.key.keysym.sym)]<<4 & 0xF0;
					Polybius.state = true;
				}
				break;


			default:
				break;
			}
		}
	}

	SDL_Quit();

	return 0;
}