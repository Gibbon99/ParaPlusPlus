#ifndef PARA_PARACONSOLE_H
#define PARA_PARACONSOLE_H

#include <string>
#include <vector>

class paraConsole
{
public:
	void add(const std::string& newLine);

	void prepare(int screenHeight, int fontHeight);

	std::vector<std::string> consoleText;
	int posX = 0;
	int posY = 0;
	std::vector<std::string>::reverse_iterator  consoleItr; // = consoleText.rbegin();
private:

};


#endif //PARA_PARACONSOLE_H
