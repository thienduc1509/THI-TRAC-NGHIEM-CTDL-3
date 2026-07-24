#include "UIController.h"

using namespace std;

// ==========================================
// CONSOLE UI PRIMITIVES
// ==========================================

void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

// ==========================================
// SAFE INPUT READER
// ==========================================

string ReadInput(int maxLength, bool allowSpace, bool isPassword) {
    string input = "";
    char ch;
    
    // 1. Ensure cursor is visible so the user knows where they are typing
    ShowConsoleCursor(true);
    
    while (true) {
        ch = getch();
        
        // 2. Handle Extended Keys (Ignore arrow keys/F-keys during text input to prevent garbage data)
        if (ch == 0 || ch == -32) {
            getch(); // Swallow the second byte of the scan code
            continue;
        }
        
        // 3. Handle ENTER (Finish typing)
        if (ch == KEY_ENTER) {
            break; 
        }
        // 4. Handle BACKSPACE (Delete character)
        else if (ch == KEY_BACKSPACE) {
            if (input.length() > 0) {
                input.pop_back(); // Remove from std::string
                // Visually erase the character on screen by moving back, printing space, moving back
                cout << "\b \b";
            }
        }
        // 5. Handle standard typing
        else if (input.length() < maxLength) {
            // Filter: Allow only printable ASCII characters (32 to 126)
            if (ch >= 32 && ch <= 126) { 
                if (ch == KEY_SPACE && !allowSpace) {
                    continue; // Block spaces if strict formatting is required
                }
                
                input += ch; // Append to std::string
                
                // Visually echo to screen
                if (isPassword) {
                    cout << "*";
                } else {
                    cout << ch;
                }
            }
        }
    }
    
    // 6. Hide cursor again after typing is complete
    ShowConsoleCursor(false);
    
    return input;
}

string NormalizeString(const string& str) {
    int start = 0;
    while (start < (int)str.length() && isspace((unsigned char)str[start])) start++;
    int end = (int)str.length() - 1;
    while (end >= 0 && isspace((unsigned char)str[end])) end--;
    
    if (start > end) return "";
    
    string result = "";
    bool inSpace = false;
    for (int i = start; i <= end; i++) {
        if (isspace((unsigned char)str[i])) {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += str[i];
            inSpace = false;
        }
    }
    return result;
}

string ToUpper(const string& str) {
    string res = str;
    for (size_t i = 0; i < res.length(); i++) {
        res[i] = toupper((unsigned char)res[i]);
    }
    return res;
}
