#include "StudentController.h"
#include <ctime>

using namespace std;

// ==========================================
// 1. EXAM ENGINE (Raw Dynamic Memory - No STL Containers)
// ==========================================

// Fisher-Yates shuffle algorithm using raw array pointer swapping (No std::random_shuffle)
void ShuffleQuestions(Question* arr, int n) {
    srand((unsigned)time(0));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Extracts questions into a raw dynamically allocated array (Question*)
Question* GetRandomQuestions(SubjectNode* subject, int numQuestions, int& actualCount) {
    int totalInList = CountQuestions(subject->data.questions);
    if (totalInList == 0) {
        actualCount = 0;
        return nullptr;
    }
    
    // Copy all questions from Singly Linked List to a raw dynamic array
    Question* pool = new Question[totalInList];
    QuestionNode* p = subject->data.questions;
    int idx = 0;
    while (p != nullptr) {
        pool[idx++] = p->data;
        p = p->next;
    }
    
    // Shuffle the raw array
    ShuffleQuestions(pool, totalInList);
    
    actualCount = (numQuestions < totalInList) ? numQuestions : totalInList;
    
    // Create result array of exact size requested
    Question* selected = new Question[actualCount];
    for (int i = 0; i < actualCount; i++) {
        selected[i] = pool[i];
    }
    
    // Free temporary pool array
    delete[] pool;
    return selected;
}

// The core exam loop featuring non-blocking timer and F10 extended key capture
void TakeExamLoop(StudentNode* student, SubjectNode* subject, Question* examQuestions, int totalQuestions, int durationMinutes) {
    int totalTimeSeconds = durationMinutes * 60;
    int currentQuestionIndex = 0;
    
    // Raw dynamic array for student answers (No std::vector)
    char* studentAnswers = new char[totalQuestions];
    for (int i = 0; i < totalQuestions; i++) {
        studentAnswers[i] = ' ';
    }
    
    time_t startTime = time(0);
    time_t endTime = startTime + totalTimeSeconds;
    
    bool isRunning = true;
    system("cls");
    
    while (isRunning) {
        // 1. TIMER CALCULATION
        time_t currentTime = time(0);
        int secondsLeft = (int)difftime(endTime, currentTime);
        
        if (secondsLeft <= 0) {
            secondsLeft = 0;
            isRunning = false; // Auto submit when time runs out
        }
        
        // 2. DRAW UI USING GOTOXY (No flickering)
        SetColor(12); // Red timer
        gotoxy(50, 2); 
        cout << "THOI GIAN CON LAI: " << (secondsLeft / 60) << " phut " << (secondsLeft % 60) << " giay   ";
        SetColor(15);
        
        gotoxy(5, 5); cout << "Cau " << (currentQuestionIndex + 1) << "/" << totalQuestions << ": " << examQuestions[currentQuestionIndex].content;
        cout << "                                                  "; 
        
        gotoxy(10, 7); cout << "A. " << examQuestions[currentQuestionIndex].A << "                                        ";
        gotoxy(10, 8); cout << "B. " << examQuestions[currentQuestionIndex].B << "                                        ";
        gotoxy(10, 9); cout << "C. " << examQuestions[currentQuestionIndex].C << "                                        ";
        gotoxy(10, 10); cout << "D. " << examQuestions[currentQuestionIndex].D << "                                        ";
        
        gotoxy(5, 12); cout << "Dap an cua ban: [ ";
        SetColor(10); cout << studentAnswers[currentQuestionIndex]; SetColor(15);
        cout << " ]";
        
        // Navigation Instructions
        gotoxy(5, 15); SetColor(11); cout << "Su dung MUI TEN TRAI / PHAI de chuyen cau hoi.";
        gotoxy(5, 16); cout << "Nhan A, B, C, hoac D de chon dap an.";
        gotoxy(5, 17); SetColor(14); cout << "Nhan F10 de NOP BAI."; SetColor(15);
        
        // 3. NON-BLOCKING KEYBOARD LISTENER
        if (_kbhit()) {
            char key = getch();
            
            if (key == 0 || key == -32) {
                key = getch(); // Read the second byte
                
                if (key == KEY_F10) { 
                    isRunning = false; // Trigger Submission
                }
                else if (key == KEY_LEFT) { 
                    if (currentQuestionIndex > 0) currentQuestionIndex--;
                    system("cls");
                }
                else if (key == KEY_RIGHT) { 
                    if (currentQuestionIndex < totalQuestions - 1) currentQuestionIndex++;
                    system("cls"); 
                }
            } else {
                char ans = toupper(key);
                if (ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') {
                    studentAnswers[currentQuestionIndex] = ans;
                }
            }
        }
        
        Sleep(50);
    }
    
    // ==========================================
    // EXAM FINISHED - CALCULATE AND SAVE SCORE
    // ==========================================
    system("cls");
    gotoxy(30, 5); SetColor(10); cout << "=== DA NOP BAI ==="; SetColor(15);
    
    int correctCount = 0;
    Score sc;
    sc.Mamh = subject->data.MAMH;
    sc.details = nullptr;
    
    for (int i = 0; i < totalQuestions; i++) {
        if (studentAnswers[i] == examQuestions[i].answer) {
            correctCount++;
        }
        
        // Save Answer Detail into Linked List
        AnswerDetail ad;
        ad.questionId = examQuestions[i].id;
        ad.studentSelection = studentAnswers[i];
        AddAnswerDetail(sc.details, ad);
    }
    
    sc.Diem = ((float)correctCount / totalQuestions) * 10.0f;
    
    // Save Score into Student's Score List
    AddScore(student->data.scores, sc);
    
    gotoxy(30, 7); cout << "So cau dung: " << correctCount << " / " << totalQuestions;
    gotoxy(30, 8); cout << "Diem cua ban: " << sc.Diem << " / 10.0";
    
    // Free raw dynamic arrays
    delete[] studentAnswers;
    delete[] examQuestions;
    
    gotoxy(30, 11); cout << "Nhan phim bat ky de quay ve Menu...";
    getch();
}

void SetupExamUI(StudentNode* student, SubjectTree rootSubjects) {
    system("cls");
    gotoxy(10, 5); cout << "Nhap Ma Mon Hoc de thi: ";
    string mamh = ReadInput(15, false, false);
    
    SubjectNode* subject = SearchSubject(rootSubjects, mamh);
    if (subject == nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Mon hoc khong ton tai!"; SetColor(15);
        Sleep(1500); return;
    }
    
    // Check if student already took this exam
    if (FindScore(student->data.scores, mamh) != nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Ban da thi mon nay roi!"; SetColor(15);
        Sleep(1500); return;
    }
    
    // Ask for parameters
    gotoxy(10, 7); cout << "Nhap so cau hoi: ";
    string sCau = ReadInput(5, false, false);
    if (sCau.empty()) return;
    int soCau = stoi(sCau);
    
    int totalAvailable = CountQuestions(subject->data.questions);
    if (soCau <= 0 || soCau > totalAvailable) {
        gotoxy(10, 9); SetColor(12); cout << "Loi: So luong cau hoi khong hop le (Toi da " << totalAvailable << ")!"; SetColor(15);
        Sleep(1500); return;
    }
    
    gotoxy(10, 8); cout << "Nhap thoi gian thi (Phut): ";
    string sPhut = ReadInput(5, false, false);
    if (sPhut.empty()) return;
    int phut = stoi(sPhut);
    
    if (phut <= 0) {
        gotoxy(10, 10); SetColor(12); cout << "Loi: Thoi gian khong hop le!"; SetColor(15);
        Sleep(1500); return;
    }
    
    gotoxy(10, 11); SetColor(14); cout << "Dang chuan bi de thi... Nhan Enter de bat dau!"; SetColor(15);
    getch();
    
    int actualCount = 0;
    Question* selectedQuestions = GetRandomQuestions(subject, soCau, actualCount);
    if (selectedQuestions != nullptr && actualCount > 0) {
        TakeExamLoop(student, subject, selectedQuestions, actualCount, phut);
    }
}

// ==========================================
// 2. STUDENT MAIN MENU
// ==========================================
void StudentMainMenu(StudentNode* student, SubjectTree& rootSubjects) {
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11);
        gotoxy(25, 3); cout << "=== MENU SINH VIEN ===";
        SetColor(14);
        gotoxy(25, 4); cout << "Xin chao, " << student->data.HO << " " << student->data.TEN;
        SetColor(15);
        
        gotoxy(30, 7); cout << "1. Thi trac nghiem";
        gotoxy(30, 8); cout << "2. Xem diem thi";
        gotoxy(30, 10); cout << "0. Dang xuat";
        gotoxy(30, 12); cout << "Chon: ";
        
        string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            SetupExamUI(student, rootSubjects);
        }
        else if (choice == "2") {
            system("cls");
            SetColor(14); gotoxy(30, 2); cout << "BANG DIEM CUA BAN"; SetColor(15);
            
            gotoxy(10, 4); cout << "MA MON HOC";
            gotoxy(40, 4); cout << "DIEM SO";
            gotoxy(10, 5); cout << "------------------------------------------";
            
            int y = 6;
            ScoreNode* p = student->data.scores;
            if (p == nullptr) {
                gotoxy(10, y); cout << "(Ban chua thi mon nao)";
            }
            while (p != nullptr) {
                gotoxy(10, y); cout << p->data.Mamh;
                gotoxy(40, y); cout << p->data.Diem;
                y++;
                p = p->next;
            }
            gotoxy(10, y + 2); cout << "Nhan phim bat ky de quay lai...";
            getch();
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 3. STUDENT LOGIN FLOW
// ==========================================
void StudentLoginFlow(ClassList& listClasses, SubjectTree& rootSubjects) {
    system("cls");
    gotoxy(30, 5); SetColor(11); cout << "=== DANG NHAP SINH VIEN ==="; SetColor(15);
    
    gotoxy(15, 7); cout << "Ma SV (Bo trong de thoat): ";
    string masv = ReadInput(15, false, false);
    if (masv.empty()) return;
    
    gotoxy(15, 8); cout << "Password (***): ";
    string pass = ReadInput(15, false, true); // true = hide characters
    
    Class* belongingClass = nullptr;
    StudentNode* student = FindStudentGlobal(listClasses, masv, belongingClass);
    
    if (student != nullptr && student->data.password == pass) {
        StudentMainMenu(student, rootSubjects);
    } else {
        gotoxy(15, 10); SetColor(12); cout << "Loi: Sai Ma SV hoac Mat khau!";
        SetColor(15);
        Sleep(1500);
    }
}
