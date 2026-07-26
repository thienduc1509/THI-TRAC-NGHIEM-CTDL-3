#include <iostream>
#include "UIController.h"
#include "FileManager.h"
#include "TeacherController.h"
#include "StudentController.h"
#include "RaylibApp.h"

using namespace std;

// ==========================================
// GLOBAL DATA STRUCTURES
// ==========================================
SubjectTree rootSubjects = nullptr;
ClassList listClasses;

// ==========================================
// SYSTEM BOOT & SHUTDOWN
// ==========================================
void BootSystem() {
    // 0. Enable UTF-8 encoding for Windows Console output
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    // 1. Initialize empty pointers
    InitClassList(listClasses);
    
    // 2. Load data from text files into memory
    LoadSubjects(rootSubjects);
    LoadQuestions(rootSubjects);
    LoadClasses(listClasses);
    LoadStudents(listClasses);
    LoadScores(listClasses);
}

void ShutdownSystem() {
    // 1. Save current memory state back to text files
    SaveSubjects(rootSubjects);
    SaveQuestions(rootSubjects);
    SaveClasses(listClasses);
    SaveStudents(listClasses);
    SaveScores(listClasses);
    
    // 2. Safely free all dynamically allocated memory
    FreeSubjectTree(rootSubjects);
    FreeClassList(listClasses);
}

// ==========================================
// MAIN MENU UI
// ==========================================
void DrawMainMenu() {
    system("cls"); // Acceptable ONLY during full screen transitions
    
    SetColor(11); // Cyan color
    gotoxy(30, 4); cout << "========================================";
    gotoxy(30, 5); cout << "|       HE THONG THI TRAC NGHIEM       |";
    gotoxy(30, 6); cout << "========================================";
    
    SetColor(14);
    gotoxy(32, 9);  cout << "1. CHAY GIAO DIEN DO HOA (Raylib GUI)";
    SetColor(15);
    gotoxy(32, 11); cout << "2. Giao Vien (Console Mode)";
    gotoxy(32, 13); cout << "3. Sinh Vien (Console Mode)";
    gotoxy(32, 15); cout << "0. Thoat";
    
    gotoxy(32, 18); cout << "Chon che do: ";
}

// ==========================================
// APPLICATION ENTRY POINT
// ==========================================
int main() {
    // 1. Boot data structures from text files
    BootSystem();
    
    // 2. Launch 100% Raylib Standalone Graphical Application
    RunRaylibApp(rootSubjects, listClasses);
    
    // 3. Save data and clean memory on close
    ShutdownSystem();
    
    return 0;
}

