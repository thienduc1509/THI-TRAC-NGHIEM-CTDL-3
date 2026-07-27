#include "raylib.h"
#include <cstdlib>

#ifndef TextToFloat
#define TextToFloat atof
#endif

#ifdef Rectangle
#undef Rectangle
#endif
#ifdef CloseWindow
#undef CloseWindow
#endif
#ifdef DrawText
#undef DrawText
#endif
#ifdef ShowCursor
#undef ShowCursor
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
