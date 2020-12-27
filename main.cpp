#include <iostream>
#include "field.hpp"
#include "ui_console.hpp"

int main(int argc, char** argv)
{
/*
	plSymbol = getPlSymbol();
	aiSymbol = plSymbol == CELL_O ? CELL_X : CELL_O;

	field.printField();
	if (plSymbol == FIRST_TURN_CELL) field.plTurn(plSymbol);

	while (true) {
		if (field.aiTurn(aiSymbol)) break;
		if (field.plTurn(plSymbol)) break;
	}
	*/
	ttt::ui::Console consoleApp;
	consoleApp.run();


	return 0;
}
