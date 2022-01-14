#pragma once

#include <SDL.h>

#define PARA_Color          SDL_Color
#define PARA_Surface        SDL_Surface
#define PARA_Texture        SDL_Texture
#define PARA_Renderer       SDL_Renderer
#define PARA_Window         SDL_Window
#define PARA_PixelFormat    SDL_PixelFormat
#define PARA_GetTicks       SDL_GetTicks
#define PARA_Thread         SDL_Thread
#define PARA_Mutex          SDL_mutex
#define PARA_CreateThread   SDL_CreateThread
#define PARA_CreateMutex    SDL_CreateMutex
#define PARA_LockMutex      SDL_LockMutex
#define PARA_UnlockMutex    SDL_UnlockMutex
#define PARA_DestroyMutex   SDL_DestroyMutex
#define PARA_Delay          SDL_Delay
#define PARA_Scancode       SDL_Scancode

#if (WIN32)
	#define myStrCpy strncpy_s
#else
	#define myStrCpy strncpy
#endif