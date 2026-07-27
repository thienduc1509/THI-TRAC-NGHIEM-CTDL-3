#include "RaylibUI.h"
#include <cstring>
#include <cctype>
#include <vector>

Font gAppFont = { 0 };

void InitVietnameseFont() {
    // Generate full list of Vietnamese Unicode Codepoints
    std::vector<int> codepoints;
    
    // ASCII Printable (32 - 126)
    for (int i = 32; i <= 126; i++) codepoints.push_back(i);
    
    // Latin-1 Supplement & Latin Extended (0x00A0 - 0x024F)
    for (int i = 0x00A0; i <= 0x024F; i++) codepoints.push_back(i);
    
    // Vietnamese Latin Extended Additional (0x1EA0 - 0x1EF9)
    for (int i = 0x1EA0; i <= 0x1EF9; i++) codepoints.push_back(i);

    // Try loading system fonts in order of preference
    const char* fontPaths[] = {
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\tahoma.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };

    for (const char* path : fontPaths) {
        if (FileExists(path)) {
            gAppFont = LoadFontEx(path, 80, codepoints.data(), (int)codepoints.size());
            if (gAppFont.texture.id > 0) {
                SetTextureFilter(gAppFont.texture, TEXTURE_FILTER_BILINEAR);
                GuiSetFont(gAppFont);
                GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
                break;
            }
        }
    }
}

void UnloadVietnameseFont() {
    if (gAppFont.texture.id > 0) {
        UnloadFont(gAppFont);
    }
}

void DrawTextCustom(const char* text, float posX, float posY, float fontSize, Color color) {
    if (gAppFont.texture.id > 0) {
        DrawTextEx(gAppFont, text, Vector2{ posX, posY }, fontSize, 1.0f, color);
    } else {
        DrawText(text, (int)posX, (int)posY, (int)fontSize, color);
    }
}

float MeasureTextCustom(const char* text, float fontSize) {
    if (gAppFont.texture.id > 0) {
        return MeasureTextEx(gAppFont, text, fontSize, 1.0f).x;
    }
    return (float)MeasureText(text, (int)fontSize);
}

bool DrawButton(Rectangle rec, const char* text, Color bg, Color textCol, bool disabled) {
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, rec) && !disabled;
    bool isClicked = isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Color drawBg = bg;
    if (disabled) {
        drawBg = GetColor(0x313244FF);
    } else if (isHovered) {
        drawBg.r = (unsigned char)((drawBg.r + 25 < 255) ? drawBg.r + 25 : 255);
        drawBg.g = (unsigned char)((drawBg.g + 25 < 255) ? drawBg.g + 25 : 255);
        drawBg.b = (unsigned char)((drawBg.b + 25 < 255) ? drawBg.b + 25 : 255);
    }

    DrawRectangleRounded(rec, 0.2f, 4, drawBg);
    DrawRectangleRoundedLines(rec, 0.2f, 4, 1.8f, isHovered ? COLOR_TEXT : COLOR_BORDER);

    float fontSize = (rec.height < 32.0f) ? 18.0f : (rec.height * 0.44f);
    if (fontSize > 28.0f) fontSize = 28.0f;

    float textWidth = MeasureTextCustom(text, fontSize);
    float textX = rec.x + (rec.width - textWidth) / 2.0f;
    float textY = rec.y + (rec.height - fontSize) / 2.0f;

    DrawTextCustom(text, textX, textY, fontSize, disabled ? COLOR_MUTED : textCol);

    return isClicked;
}

bool DrawTextInput(Rectangle rec, char* buffer, int maxLen, bool& isActive, const char* placeholder, bool isPassword) {
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, rec);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isActive = isHovered;
    }

    if (isActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && ((int)strlen(buffer) < maxLen - 1)) {
                int len = (int)strlen(buffer);
                buffer[len] = (char)key;
                buffer[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            int len = (int)strlen(buffer);
            if (len > 0) {
                buffer[len - 1] = '\0';
            }
        }
    }

    Color borderColor = isActive ? COLOR_PRIMARY : (isHovered ? COLOR_MUTED : COLOR_BORDER);
    DrawRectangleRounded(rec, 0.15f, 4, COLOR_CARD);
    DrawRectangleRoundedLines(rec, 0.15f, 4, 1.8f, borderColor);

    float fontSize = (rec.height < 32.0f) ? 18.0f : (rec.height * 0.44f);
    if (fontSize > 26.0f) fontSize = 26.0f;

    float textY = rec.y + (rec.height - fontSize) / 2.0f;

    if (strlen(buffer) == 0 && !isActive && placeholder && strlen(placeholder) > 0) {
        DrawTextCustom(placeholder, rec.x + 14, textY, fontSize, COLOR_MUTED);
    } else {
        std::string displayText = buffer;
        if (isPassword) {
            displayText = std::string(strlen(buffer), '*');
        }

        DrawTextCustom(displayText.c_str(), rec.x + 14, textY, fontSize, COLOR_TEXT);

        if (isActive && ((int)(GetTime() * 2.5f) % 2 == 0)) {
            float cursorX = rec.x + 14 + MeasureTextCustom(displayText.c_str(), fontSize);
            DrawRectangle((int)cursorX + 2, (int)textY, 2, (int)fontSize, COLOR_PRIMARY);
        }
    }

    return isActive;
}

void DrawCard(Rectangle rec, Color bg, Color border) {
    DrawRectangleRounded(rec, 0.08f, 4, bg);
    DrawRectangleRoundedLines(rec, 0.08f, 4, 1.8f, border);
}

void DrawHeader(const char* title, const char* subtitle, float sw) {
    DrawTextCustom(title, 40, 20, 32.0f, COLOR_PRIMARY);
    if (subtitle && strlen(subtitle) > 0) {
        DrawTextCustom(subtitle, 40, 60, 20.0f, COLOR_MUTED);
    }
    DrawLine(40, 90, (int)(sw - 40), 90, COLOR_BORDER);
}
