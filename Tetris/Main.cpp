#include "Tetris.h"

int main()
{
	initButtons();
	initgraph(CELL * 16, CELL * 20, NOCLOSE);
	Tetris();
	return 0;
}
