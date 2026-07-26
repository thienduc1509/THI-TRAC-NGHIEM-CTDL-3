#pragma once
#include "raylib.h"

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

#include "Models.h"
#include <string>

// ==========================================
// COLOR PALETTE (Modern Slate Dark Theme)
// ==========================================
#define COLOR_BG          GetColor(0x181825FF)  // Deep Dark Slate
#define COLOR_CARD        GetColor(0x1E1E2EFF)  // Surface Card
#define COLOR_CARD_HOVER  GetColor(0x313244FF)  // Hover Card
#define COLOR_TEXT        GetColor(0xCDD6F4FF)  // Main Text White/Ice
#define COLOR_MUTED       GetColor(0xBAC2DEFF)  // Subtitle Muted Gray
#define COLOR_PRIMARY     GetColor(0x89B4FAFF)  // Vibrant Accent Blue
#define COLOR_SUCCESS     GetColor(0xA6E3A1FF)  // Success Emerald Green
#define COLOR_DANGER      GetColor(0xF38BA8FF)  // Coral Red
#define COLOR_WARNING     GetColor(0xF9E2AFFF)  // Amber Yellow
#define COLOR_BORDER      GetColor(0x45475AFF)  // Border Gray

// Global App Font with full Vietnamese Unicode Codepoints
extern Font gAppFont;

// ==========================================
// CUSTOM RAYLIB GUI CONTROLS & UTILS
// ==========================================

// Init Vietnamese Unicode TTF Font
void InitVietnameseFont();

// Unload App Font
void UnloadVietnameseFont();

// Custom DrawText supporting UTF-8 Vietnamese & custom font
void DrawTextCustom(const char* text, float posX, float posY, float fontSize, Color color);

// Measure width of custom UTF-8 text
float MeasureTextCustom(const char* text, float fontSize);

// Draws a sleek modern button with hover/active state feedback. Returns true when clicked.
bool DrawButton(Rectangle rec, const char* text, Color bg = COLOR_PRIMARY, Color textCol = COLOR_BG, bool disabled = false);

// Draws a text input box. Returns true if key pressed/modified.
bool DrawTextInput(Rectangle rec, char* buffer, int maxLen, bool& isActive, const char* placeholder = "", bool isPassword = false);

// Draws a rounded card container with optional border
void DrawCard(Rectangle rec, Color bg = COLOR_CARD, Color border = COLOR_BORDER);

// Header title banner for responsive screens
void DrawHeader(const char* title, const char* subtitle, float sw);
