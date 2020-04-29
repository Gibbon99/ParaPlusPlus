#include <game/audio.h>
#include "../../hdr/io/consoleFunctions.h"

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

void testPlay ()
{
	gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "start1");

	//audio.play ("start1", false, 0, 127);
}