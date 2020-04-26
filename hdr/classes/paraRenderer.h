#ifndef PARA_PARARENDERER_H
#define PARA_PARARENDERER_H

#include <string>
#include <map>
#include "../wrapper.h"

struct __backingTexture
{
	PARA_Texture *backingTexture;
	int          logicalWidth;
	int          logicalHeight;
};

struct __rendererInfo
{
	SDL_RendererInfo rendererInfo;
	std::string      rendererName            = "";
	bool             softwareFallback        = false;
	bool             hardwareAccelerated     = false;
	bool             supportsRenderToTexture = false;
	bool             supportsVSync           = false;
};

typedef void (*functionPtrStr) (std::string);

typedef void (*functionPtrOut) (int, std::string);

class paraRenderer
{
public:

	paraRenderer ();

	// Create things
	void create (int newWinWidth, int newWinHeight, int winFlags, int rendererIndex, bool useVSync, const std::string &windowTitle);

	void createRenderTargetTexture (const std::string &textureName, int logicalWidth, int logicalHeight, int setRenderScaleQuality);

	Uint32 createRendererFlags (int rendererIndex);

	// Get things
	void getRendererInfo ();

	std::string getCurrentBackingTexture ();

	PARA_Texture *getRenderTarget (const std::string &textureName);

	// Set things
	void setShutdownFunction (functionPtrStr outputFunction);

	void setConOutFunction (functionPtrOut outputFunction);

	void setCurrentBackingTexture (const std::string &newActiveTexture);

	void useNewRenderer (int newRendererIndex);

	// Misc
	void prepareFrame ();

	void presentFrame ();

	// Debug
	void d_getAllBackingTextures ();

	void d_getCurrentRenderer ();

	void d_getCurrentBackingTexture ();

	// Display all the available renderers
	void d_getAllRenderers ();

	// Utils
	void verifyRenderer ();

	std::string int_getString (std::string format, ...);

	// The window we'll be rendering to
	PARA_Window   *window                                          = nullptr;
	// Renderer associated with the window
	PARA_Renderer *renderer                                        = nullptr;
private:
	int                                     windowWidth;
	int                                     windowHeight;
	int                                     cacheWinFlags;
	int                                     whichRenderer;
	bool                                    int_useVSync;
	bool                                    targetTextureAvailable = false;
	functionPtrStr                          shutdownFunc;
	functionPtrOut                          consoleOutFunc;
	std::vector<__rendererInfo>             rendererInfo;
	std::map<std::string, __backingTexture> backingTextures;
	std::string                             activeBackingTexture;
	std::string                             cacheTitle;
};

#endif //PARA_PARARENDERER_H
