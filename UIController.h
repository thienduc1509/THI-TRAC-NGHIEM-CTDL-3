#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

// ==========================================
// CONSTANTS: KEY CODES
// ==========================================
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;
const int KEY_BACKSPACE = 8;
const int KEY_SPACE = 32;
const int KEY_F10 = 68; // 2nd byte of F10 extended key code

// ==========================================
// CONSOLE UI PRIMITIVES
// ==========================================

// Moves the cursor to a specific (x, y) coordinate. Used to draw UI without `system("cls")`.
void gotoxy(int x, int y);

// Changes the text color (0-15 standard Windows console colors)
void SetColor(int color);

// Hides or shows the blinking cursor underscore
void ShowConsoleCursor(bool showFlag);

// ==========================================
// SAFE INPUT READER
// ==========================================
// Sinh vien beginner note:
// Never use standard `cin >>` for UI applications! It breaks the screen if the user types too much.
// We capture keyboard inputs character-by-character using `getch()` so we can strictly enforce:
// 1. maxLength: Prevents buffer overflow and stops text from bleeding out of UI boxes.
// 2. allowSpace: E.g., Student ID "N18" should not allow spaces.
// 3. isPassword: E.g., Student passwords should echo '*' instead of real characters.
// NOTE: If the user just presses ENTER immediately, this returns an empty string "".
std::string ReadInput(int maxLength, bool allowSpace = true, bool isPassword = false);

// Removes extra leading/trailing spaces and collapses multiple spaces into a single space
std::string NormalizeString(const std::string& str);

// Converts string to UPPERCASE for strict code matching (MAMH, MALOP, MASV)
std::string ToUpper(const std::string& str);
