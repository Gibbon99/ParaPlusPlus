#include "gui/guiLanguage.h"

int currentLanguage;        // From config file

typedef struct
{
	std::string		key;
	std::string		textString;
} _textStrings;

std::vector<_textStrings>		textStrings;

//--------------------------------------------------------
//
// Return a string based on the key
std::string gui_getString(string& key)
//--------------------------------------------------------
{
	for (auto & textString : textStrings)
	{
		if (key == textString.key)
		{
			return textString.textString;
		}
	}
	return key + " not found";
}

//--------------------------------------------------------
//
// Add a key and string to the map
void gui_addKeyAndText(std::string &key, std::string &textString)
//--------------------------------------------------------
{
	_textStrings		tmpString;

	tmpString.key = key;
	tmpString.textString = textString;

	textStrings.push_back(tmpString);
}