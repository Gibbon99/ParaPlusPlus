#include "game/shipDecks.h"
#include "io/consoleFunctions.h"

void debug_showAIValues()
{
	shipdecks.at (gam_getCurrentDeckName ()).droid[0].ai2.debugShowValues ();
}

void debug_getCurrentBackingTexture()
{
	renderer.d_getCurrentBackingTexture ();
}

void debug_getAllBackingTextures()
{
	renderer.d_getAllBackingTextures ();
}

void debug_getRenderInfo()
{
	renderer.d_getCurrentRenderer ();
};

void debug_getAllRenderers()
{
	renderer.d_getAllRenderers ();
}
