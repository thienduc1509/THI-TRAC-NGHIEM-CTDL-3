#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTER 13
#define BACKSPACE 8

// 1. Nhập số
int NhapSoTungKySo() {
    char str[15];
    int i = 0;
    char ch;
    
    while (1) {
        ch = getch();
        
        if (ch >= '0' && ch <= '9') {
            if (i < 9) { 
                str[i++] = ch; 
            }
        }
        else if (ch == BACKSPACE && i > 0) {
            i--;
        }
        else if (ch == ENTER && i > 0) {
            break;
        }
    }
    str[i] = '\0'; 
    return atoi(str);
}

// 2. Nhập chuỗi toàn chữ và blank (Trả về 1: Thành công, 0: Quá chiều dài)
int NhapChuoiToanChuVaBlank(char *S, int maxLength) {
    int i = 0;
    char ch;
    
    while (1) {
        ch = getch();
        
        bool isLetter = ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
        bool isSpace = (ch == ' ');
        
        if (isLetter || isSpace) {
            // Chặn khoảng trắng thừa
            if (isSpace) {
                if (i == 0) continue; 
                if (i > 0 && S[i-1] == ' ') continue;
            }
            
            if (i >= maxLength - 1) {
                return 0; 
            }
            
            printf("%c", ch);
            S[i++] = ch;
        }
        else if (ch == BACKSPACE && i > 0) {
            printf("\b \b");
            i--;
        }
        else if (ch == ENTER && i > 0) {
            if (S[i-1] == ' ') {
                printf("\b \b");
                i--;
            }
            break;
        }
    }
    S[i] = '\0';
    return 1; 
}

//3.Nhập mã (kiểu chuỗi): chỉ nhận chữ cái, ký số, dấu -, dấu _. Chữ cái tự động ra chữ in (Trả về 1: Thành công, 0: Quá chiều dài)
int NhapMaChuoi(char *S, int maxLength) {
    int i = 0;
    char ch;
    
    while (1) {
        ch = getch();
        
        bool isValidChar = ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || 
                            (ch >= '0' && ch <= '9') || ch == '-' || ch == '_');
        
        if (isValidChar) {
            if (i >= maxLength - 1) {
                return 0;
            }
            
            if (ch >= 'a' && ch <= 'z') {
                ch -= 32; 
            }
            printf("%c", ch);
            S[i++] = ch;
        }
        else if (ch == BACKSPACE && i > 0) {
            printf("\b \b");
            i--;
        }
        else if (ch == ENTER && i > 0) {
            break;
        }
    }
    S[i] = '\0';
    return 1;
}