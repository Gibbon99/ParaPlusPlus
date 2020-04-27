
//----------------------------------------------------------------------------------------------------------------------
//
// Change to using a new renderer - called from host program
//
// Pass in the index of the new renderer
void as_useNewRenderer(int &in newRenderer)
//----------------------------------------------------------------------------------------------------------------------
{
//	as_renderer.getRendererInfo();
//	as_renderer.d_getAllRenderers();

string newIndex;

	newIndex = formatInt(newRenderer, "l");

//sys_printConInt(newIndex, 99);

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_USE_NEW_RENDERER, 0, newIndex);
}

void as_testFunction()
{
	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, "Text from inside the script");
}