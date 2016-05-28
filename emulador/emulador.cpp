// emulador.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "CPU.h"
#include <iostream>
using namespace std;
int main()
{
	CPU polybius;
	polybius.loadROM(Path);
	while (true)
	{
		polybius.execute();
	}

    return 0;
}

