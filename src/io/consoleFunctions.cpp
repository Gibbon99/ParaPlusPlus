//
// Created by dberry on 26/4/20.
//

#include "../../hdr/io/consoleFunctions.h"
#include "../../hdr/main.h"

void debug_getCurrentBackingTexture ()
{
	renderer.d_getCurrentBackingTexture ();
}

void debug_getAllBackingTextures ()
{
	renderer.d_getAllBackingTextures ();
}

void debug_getRenderInfo ()
{
	renderer.d_getCurrentRenderer ();
};

void debug_getAllRenderers ()
{
	renderer.d_getAllRenderers ();
}


