// emulador.cpp: define el punto de entrada de la aplicaci�n de consola.
//

#include "stdafx.h"
#include "CPU.h"
#include <iostream>
using namespace std;
int main()
{
	CPU cpu;
	cpu.execute();

    return 0;
}

