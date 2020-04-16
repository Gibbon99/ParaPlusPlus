#include "../../hdr/classes/paraConsole.h"

void paraConsole::add(const std::string& newLine)
{
	paraConsole::consoleText.push_back(newLine);
}

void paraConsole::prepare(int screenHeight, int fontHeight)
{
	paraConsole::consoleItr = paraConsole::consoleText.rbegin();
	paraConsole::posX = 0;
	paraConsole::posY = screenHeight - (fontHeight * 2);
}