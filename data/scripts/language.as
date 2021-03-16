//
// Select which language to assign to strings
//

//------------------------------------------------------------
//
// Choose language and set strings
void as_setLanguageStrings()
//------------------------------------------------------------
{
	switch (currentLanguage)
	{
		case LANG_ENGLISH:
			as_setEnglishText();
			break;

		case LANG_ITALIAN:
			as_setItalianText();
			break;

		case LANG_GERMAN:
			as_setGermanText();
			break;
	}
}