#include "../../hdr/classes/paraConsole.h"

void paraConsole::add(const std::string& newLine)
{
	paraConsole::consoleText.push_back(newLine);
}

void paraConsole::prepare(int newPosX, int newPosY)
{
	paraConsole::consoleItr = paraConsole::consoleText.rbegin();
	paraConsole::posX = newPosX;
	paraConsole::posY = newPosY;
}