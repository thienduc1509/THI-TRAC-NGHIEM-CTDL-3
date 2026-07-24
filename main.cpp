#include <iostream>
#include "UIController.h"
#include "FileManager.h"
#include "TeacherController.h"
#include "StudentController.h"

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
    gotoxy(30, 5); cout << "=====================================";
    gotoxy(30, 6); cout << "|      HE THONG THI TRAC NGHIEM     |";
    gotoxy(30, 7); cout << "=====================================";
    
    SetColor(15); // White color
    gotoxy(35, 10); cout << "1. Giao Vien (Teacher Mode)";
    gotoxy(35, 12); cout << "2. Sinh Vien (Student Mode)";
    gotoxy(35, 14); cout << "0. Thoat (Exit)";
    
    gotoxy(35, 18); cout << "Chon chuc nang: ";
}

// ==========================================
// APPLICATION ENTRY POINT
// ==========================================
int main() {
    // Phase 1 integration: Load Data
    BootSystem();
    
    bool isRunning = true;
    while (isRunning) {
        DrawMainMenu();
        
        // Phase 1 integration: Use safe input reader
        // Max length 1, no spaces allowed, not a password
        string choice = ReadInput(1, false, false); 
        
        if (choice == "1") {
            TeacherMainMenu(rootSubjects, listClasses);
        }
        else if (choice == "2") {
            StudentLoginFlow(listClasses, rootSubjects);
        }
        else if (choice == "0") {
            isRunning = false; 
        }
    }
    
    // Phase 1 integration: Save Data & Clean Memory
    ShutdownSystem();
    
    return 0;
}
