#include "TeacherController.h"

// ==========================================
// 1. MANAGE SUBJECTS (UI For Binary Search Tree)
// ==========================================
void ManageSubjectsUI(SubjectTree& rootSubjects) {
    bool isRunning = true;
    while (isRunning) {
        system("cls"); // Safe to clear screen for full menus
        
        SetColor(11);
        gotoxy(30, 2); cout << "=== QUAN LY MON HOC (BST) ===";
        
        SetColor(15);
        gotoxy(10, 5); cout << "1. Xem danh sach mon hoc (In-order Traversal)";
        gotoxy(10, 6); cout << "2. Them mon hoc";
        gotoxy(10, 7); cout << "3. Hieu chinh mon hoc (Batch Edit Flow)";
        gotoxy(10, 8); cout << "4. Xoa mon hoc";
        gotoxy(10, 10); cout << "0. Quay lai";
        gotoxy(10, 12); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            DrawSubjectList(rootSubjects);
            
            gotoxy(5, 25); cout << "Nhan phim bat ky de quay lai...";
            getch();
        }
        else if (choice == "2") {
            system("cls");
            Subject s;
            gotoxy(10, 5); cout << "Nhap Ma Mon Hoc (Khong chua khoang trang): ";
            s.MAMH = ReadInput(15, false, false); // No spaces allowed
            if (s.MAMH.empty()) continue; // Abort if they press enter
            
            gotoxy(10, 6); cout << "Nhap Ten Mon Hoc: ";
            s.TENMH = NormalizeString(ReadInput(50, true, false)); // Normalize extra spaces
            if (s.TENMH.empty()) continue;
            
            if (InsertSubject(rootSubjects, s)) {
                gotoxy(10, 8); SetColor(10); cout << "Them thanh cong!";
            } else {
                gotoxy(10, 8); SetColor(12); cout << "Loi: Ma Mon Hoc hoac Ten Mon Hoc da ton tai!";
            }
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "3") {
            // PHASE 1 DESIGN REQUIREMENT: BATCH EDIT FLOW
            system("cls");
            gotoxy(10, 5); cout << "Nhap Ma Mon Hoc can hieu chinh: ";
            std::string mamh = ReadInput(15, false, false);
            
            SubjectNode* p = SearchSubject(rootSubjects, mamh);
            if (p != nullptr) {
                gotoxy(10, 7); cout << "Ten hien tai: " << p->data.TENMH;
                gotoxy(10, 8); cout << "Ten moi (Bo trong va an Enter de giu nguyen): ";
                
                std::string newTen = NormalizeString(ReadInput(50, true, false));
                
                // Check if new name already exists elsewhere
                SubjectNode* dupNode = SearchSubjectByName(rootSubjects, newTen);
                if (dupNode != nullptr && dupNode->data.MAMH != p->data.MAMH) {
                    gotoxy(10, 10); SetColor(12); cout << "Loi: Ten Mon Hoc nay da ton tai!";
                } else if (!newTen.empty()) {
                    p->data.TENMH = newTen;
                    gotoxy(10, 10); SetColor(10); cout << "Cap nhat thanh cong!";
                } else {
                    gotoxy(10, 10); SetColor(14); cout << "Khong co thay doi. Giu nguyen ten cu.";
                }
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay mon hoc!";
            }
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "4") {
            system("cls");
            gotoxy(10, 5); cout << "Nhap Ma Mon Hoc can xoa: ";
            std::string mamh = ReadInput(15, false, false);
            
            if (DeleteSubject(rootSubjects, mamh)) {
                gotoxy(10, 7); SetColor(10); cout << "Xoa thanh cong!";
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay mon hoc de xoa!";
            }
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 2. HELPER: DRAW SUBJECT LIST
// ==========================================
void DrawSubjectList(SubjectTree rootSubjects) {
    system("cls");
    SetColor(14);
    gotoxy(30, 2); cout << "DANH SACH MON HOC";
    SetColor(15);
    
    // We collect the BST nodes into an array using In-order traversal
    // so they are sorted alphabetically by MAMH.
    SubjectNode* arr[1000];
    int count = 0;
    CollectSubjectsInOrder(rootSubjects, arr, count);
    
    gotoxy(5, 4); cout << "STT";
    gotoxy(15, 4); cout << "MA MON HOC";
    gotoxy(35, 4); cout << "TEN MON HOC";
    gotoxy(5, 5); cout << "---------------------------------------------------------";
    
    int y = 6;
    for (int i = 0; i < count; i++) {
        gotoxy(5, y); cout << (i + 1);
        gotoxy(15, y); cout << arr[i]->data.MAMH;
        gotoxy(35, y); cout << arr[i]->data.TENMH;
        y++;
    }
}

// ==========================================
// 3. MANAGE QUESTIONS (UI For Linked List inside BST)
// ==========================================

void GetMaxQuestionId(SubjectTree root, int& maxId) {
    if (root != nullptr) {
        QuestionNode* q = root->data.questions;
        while (q != nullptr) {
            if (q->data.id > maxId) maxId = q->data.id;
            q = q->next;
        }
        GetMaxQuestionId(root->left, maxId);
        GetMaxQuestionId(root->right, maxId);
    }
}

int GenerateNewQuestionId(SubjectTree rootSubjects) {
    int maxId = 0;
    GetMaxQuestionId(rootSubjects, maxId);
    return maxId + 1;
}

void DrawQuestionList(SubjectNode* subject) {
    system("cls");
    SetColor(14);
    gotoxy(30, 2); cout << "DANH SACH CAU HOI: " << subject->data.TENMH;
    SetColor(15);
    
    gotoxy(5, 4); cout << "ID";
    gotoxy(12, 4); cout << "NOI DUNG";
    gotoxy(70, 4); cout << "DAP AN";
    gotoxy(5, 5); cout << "-------------------------------------------------------------------------";
    
    int y = 6;
    QuestionNode* p = subject->data.questions;
    while (p != nullptr) {
        gotoxy(5, y); cout << p->data.id;
        // Print just the first 50 chars of the question to fit screen
        std::string preview = p->data.content;
        if (preview.length() > 50) preview = preview.substr(0, 47) + "...";
        gotoxy(12, y); cout << preview;
        
        gotoxy(72, y); SetColor(10); cout << p->data.answer; SetColor(15);
        y++;
        p = p->next;
        
        // Simple pagination
        if (y > 20) {
            gotoxy(5, y + 1); cout << "Nhan phim bat ky de xem tiep...";
            getch();
            system("cls");
            y = 6;
        }
    }
}

void ManageQuestionsUI(SubjectTree rootSubjects) {
    system("cls");
    gotoxy(10, 5); cout << "Nhap Ma Mon Hoc de quan ly cau hoi: ";
    std::string mamh = ReadInput(15, false, false);
    
    // 1. Traverse BST to find the specific subject
    SubjectNode* p = SearchSubject(rootSubjects, mamh);
    if (p == nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay mon hoc!";
        SetColor(15);
        Sleep(1500);
        return; // Kick back to Main Menu
    }
    
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11);
        gotoxy(25, 2); cout << "=== QUAN LY CAU HOI: " << p->data.TENMH << " ===";
        SetColor(15);
        
        gotoxy(10, 5); cout << "1. Xem danh sach cau hoi";
        gotoxy(10, 6); cout << "2. Them cau hoi thi";
        gotoxy(10, 7); cout << "3. Hieu chinh cau hoi (Batch Edit)";
        gotoxy(10, 8); cout << "4. Xoa cau hoi";
        gotoxy(10, 10); cout << "0. Quay lai";
        gotoxy(10, 12); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            DrawQuestionList(p);
            gotoxy(5, 25); cout << "Nhan phim bat ky de quay lai...";
            getch();
        }
        else if (choice == "2") {
            system("cls");
            Question q;
            // Generates a globally unique ID traversing all subjects
            q.id = GenerateNewQuestionId(rootSubjects); 
            
            gotoxy(10, 5); cout << "[ID Tu Dong]: " << q.id;
            gotoxy(10, 6); cout << "Nhap Noi Dung Cau Hoi: ";
            q.content = ReadInput(200, true, false);
            if (q.content.empty()) continue; // Abort on empty enter
            
            gotoxy(10, 7); cout << "Dap an A: "; q.A = ReadInput(100, true, false);
            gotoxy(10, 8); cout << "Dap an B: "; q.B = ReadInput(100, true, false);
            gotoxy(10, 9); cout << "Dap an C: "; q.C = ReadInput(100, true, false);
            gotoxy(10, 10); cout << "Dap an D: "; q.D = ReadInput(100, true, false);
            
            // Loop until they input a valid character A, B, C, or D
            while(true) {
                gotoxy(10, 11); cout << "Dap an Dung (A/B/C/D): "; 
                std::string ans = ReadInput(1, false, false);
                if (!ans.empty()) {
                    char ansChar = toupper(ans[0]);
                    if (ansChar == 'A' || ansChar == 'B' || ansChar == 'C' || ansChar == 'D') {
                        q.answer = ansChar;
                        break;
                    }
                }
                gotoxy(10, 12); SetColor(12); cout << "Loi: Chi duoc nhap A, B, C, hoac D!"; SetColor(15);
                Sleep(1000);
                gotoxy(10, 12); cout << "                                         "; // Clear error
                gotoxy(33, 11); cout << " "; // Clear input
            }
            
            // Append to Singly Linked List
            AddQuestion(p->data.questions, q);
            gotoxy(10, 13); SetColor(10); cout << "Them thanh cong!";
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "3") {
            system("cls");
            gotoxy(10, 5); cout << "Nhap ID cau hoi can sua: ";
            std::string idStr = ReadInput(5, false, false);
            if (idStr.empty()) continue;
            int qId = stoi(idStr);
            
            QuestionNode* qNode = FindQuestion(p->data.questions, qId);
            if (qNode != nullptr) {
                gotoxy(10, 7); SetColor(14); cout << "[Bat che do Batch Edit - Bo trong an Enter de giu nguyen]"; SetColor(15);
                
                gotoxy(10, 9); cout << "Noi Dung (" << qNode->data.content.substr(0, 15) << "...): ";
                std::string newContent = ReadInput(200, true, false);
                if (!newContent.empty()) qNode->data.content = newContent;
                
                gotoxy(10, 10); cout << "A (" << qNode->data.A << "): ";
                std::string newA = ReadInput(100, true, false);
                if (!newA.empty()) qNode->data.A = newA;
                
                gotoxy(10, 11); cout << "B (" << qNode->data.B << "): ";
                std::string newB = ReadInput(100, true, false);
                if (!newB.empty()) qNode->data.B = newB;
                
                gotoxy(10, 12); cout << "C (" << qNode->data.C << "): ";
                std::string newC = ReadInput(100, true, false);
                if (!newC.empty()) qNode->data.C = newC;
                
                gotoxy(10, 13); cout << "D (" << qNode->data.D << "): ";
                std::string newD = ReadInput(100, true, false);
                if (!newD.empty()) qNode->data.D = newD;
                
                gotoxy(10, 14); cout << "Dap An Dung (" << qNode->data.answer << "): ";
                std::string newAns = ReadInput(1, false, false);
                if (!newAns.empty()) {
                    char ansChar = toupper(newAns[0]);
                    if (ansChar == 'A' || ansChar == 'B' || ansChar == 'C' || ansChar == 'D') {
                        qNode->data.answer = ansChar;
                    }
                }
                
                gotoxy(10, 16); SetColor(10); cout << "Cap nhat thanh cong!";
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay ID cau hoi!";
            }
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "4") {
            system("cls");
            gotoxy(10, 5); cout << "Nhap ID cau hoi can xoa: ";
            std::string idStr = ReadInput(5, false, false);
            if (idStr.empty()) continue;
            
            if (DeleteQuestion(p->data.questions, stoi(idStr))) {
                gotoxy(10, 7); SetColor(10); cout << "Xoa thanh cong!";
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay ID!";
            }
            SetColor(15);
            Sleep(1500);
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 5. MANAGE CLASSES (UI For Pointer Array)
// ==========================================
void DrawClassList(ClassList listClasses) {
    system("cls");
    SetColor(14);
    gotoxy(30, 2); cout << "DANH SACH LOP HOC";
    SetColor(15);
    
    gotoxy(5, 4); cout << "STT";
    gotoxy(15, 4); cout << "MA LOP";
    gotoxy(35, 4); cout << "TEN LOP";
    gotoxy(5, 5); cout << "---------------------------------------------------------";
    
    int y = 6;
    int index = 1;
    for (int i = 0; i < listClasses.size; i++) {
        if (listClasses.nodes[i] != nullptr) {
            gotoxy(5, y); cout << index++;
            gotoxy(15, y); cout << listClasses.nodes[i]->MALOP;
            gotoxy(35, y); cout << listClasses.nodes[i]->TENLOP;
            y++;
        }
    }
}

void ManageClassesUI(ClassList& listClasses) {
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11); gotoxy(30, 2); cout << "=== QUAN LY LOP HOC ==="; SetColor(15);
        
        gotoxy(10, 5); cout << "1. Xem danh sach lop";
        gotoxy(10, 6); cout << "2. Them lop";
        gotoxy(10, 7); cout << "3. Hieu chinh lop";
        gotoxy(10, 9); cout << "0. Quay lai";
        gotoxy(10, 11); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            DrawClassList(listClasses);
            gotoxy(5, 25); cout << "Nhan phim bat ky de quay lai...";
            getch();
        }
        else if (choice == "2") {
            system("cls");
            Class c;
            gotoxy(10, 5); cout << "Nhap Ma Lop (Khong khoang trang): ";
            c.MALOP = ReadInput(15, false, false);
            if (c.MALOP.empty()) continue;
            
            gotoxy(10, 6); cout << "Nhap Ten Lop: ";
            c.TENLOP = NormalizeString(ReadInput(50, true, false));
            if (c.TENLOP.empty()) continue;
            
            if (AddClass(listClasses, c)) {
                gotoxy(10, 8); SetColor(10); cout << "Them lop thanh cong!";
            } else {
                gotoxy(10, 8); SetColor(12); cout << "Loi: Ma Lop hoac Ten Lop da ton tai!";
            }
            SetColor(15); Sleep(1500);
        }
        else if (choice == "3") {
            system("cls");
            gotoxy(10, 5); cout << "Nhap Ma Lop can hieu chinh: ";
            std::string malop = ReadInput(15, false, false);
            
            Class* c = FindClass(listClasses, malop);
            if (c != nullptr) {
                gotoxy(10, 7); SetColor(14); cout << "[Batch Edit - Bo trong an Enter de giu nguyen]"; SetColor(15);
                gotoxy(10, 9); cout << "Ten lop hien tai (" << c->TENLOP << "): ";
                std::string newTen = ReadInput(50, true, false);
                if (!newTen.empty()) {
                    c->TENLOP = newTen;
                    gotoxy(10, 11); SetColor(10); cout << "Cap nhat thanh cong!";
                } else {
                    gotoxy(10, 11); SetColor(14); cout << "Giu nguyen ten cu.";
                }
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay lop!";
            }
            SetColor(15); Sleep(1500);
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 6. MANAGE STUDENTS (UI For Linked List)
// ==========================================
void DrawStudentList(Class* c) {
    system("cls");
    SetColor(14);
    gotoxy(25, 2); cout << "DANH SACH SINH VIEN LOP: " << c->TENLOP;
    SetColor(15);
    
    gotoxy(5, 4); cout << "STT";
    gotoxy(12, 4); cout << "MASV";
    gotoxy(25, 4); cout << "HO";
    gotoxy(45, 4); cout << "TEN";
    gotoxy(60, 4); cout << "PHAI";
    gotoxy(5, 5); cout << "-------------------------------------------------------------------------";
    
    int y = 6;
    int index = 1;
    StudentNode* p = c->students;
    while (p != nullptr) {
        gotoxy(5, y); cout << index++;
        gotoxy(12, y); cout << p->data.MASV;
        gotoxy(25, y); cout << p->data.HO;
        gotoxy(45, y); cout << p->data.TEN;
        gotoxy(60, y); cout << p->data.PHAI;
        y++;
        p = p->next;
        
        if (y > 20) {
            gotoxy(5, y + 1); cout << "Nhan phim bat ky de xem tiep...";
            getch();
            system("cls");
            y = 6;
        }
    }
}

void ManageStudentsUI(ClassList& listClasses) {
    system("cls");
    gotoxy(10, 5); cout << "Nhap Ma Lop de quan ly sinh vien: ";
    std::string malop = ReadInput(15, false, false);
    
    Class* c = FindClass(listClasses, malop);
    if (c == nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Khong tim thay lop hoc!";
        SetColor(15); Sleep(1500);
        return;
    }
    
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11); gotoxy(25, 2); cout << "=== QUAN LY SINH VIEN LOP: " << c->MALOP << " ==="; SetColor(15);
        
        gotoxy(10, 5); cout << "1. Xem danh sach sinh vien";
        gotoxy(10, 6); cout << "2. Them sinh vien";
        gotoxy(10, 7); cout << "3. Hieu chinh thong tin sinh vien";
        gotoxy(10, 9); cout << "0. Quay lai";
        gotoxy(10, 11); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            DrawStudentList(c);
            gotoxy(5, 25); cout << "Nhan phim bat ky de quay lai...";
            getch();
        }
        else if (choice == "2") {
            system("cls");
            Student s;
            gotoxy(10, 5); cout << "Nhap Ma SV (Khong khoang trang): ";
            s.MASV = ToUpper(ReadInput(15, false, false));
            if (s.MASV.empty()) continue;
            
            Class* tempClass = nullptr;
            if (FindStudentGlobal(listClasses, s.MASV, tempClass) != nullptr) {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Ma SV nay da ton tai trong he thong!";
                SetColor(15); Sleep(1500);
                continue;
            }
            
            gotoxy(10, 6); cout << "Nhap Ho: "; s.HO = NormalizeString(ReadInput(30, true, false));
            gotoxy(10, 7); cout << "Nhap Ten: "; s.TEN = NormalizeString(ReadInput(15, false, false));
            
            while(true) {
                gotoxy(10, 8); cout << "Nhap Phai (Nam/Nu): ";
                std::string phai = ReadInput(5, false, false);
                if (phai == "Nam" || phai == "nam" || phai == "NAM") { s.PHAI = "Nam"; break; }
                if (phai == "Nu" || phai == "nu" || phai == "NU") { s.PHAI = "Nu"; break; }
                gotoxy(10, 9); SetColor(12); cout << "Loi: Chi nhap Nam hoac Nu!"; SetColor(15);
                Sleep(1000); gotoxy(10, 9); cout << "                                ";
                gotoxy(30, 8); cout << "       "; // Clear input
            }
            
            gotoxy(10, 10); cout << "Nhap Password: ";
            s.password = ReadInput(15, false, true); // isPassword = true
            
            AddStudent(c->students, s);
            gotoxy(10, 12); SetColor(10); cout << "Them sinh vien thanh cong!";
            SetColor(15); Sleep(1500);
        }
        else if (choice == "3") {
            system("cls");
            gotoxy(10, 5); cout << "Nhap Ma SV can hieu chinh: ";
            std::string masv = ReadInput(15, false, false);
            
            StudentNode* sNode = FindStudent(c->students, masv);
            if (sNode != nullptr) {
                gotoxy(10, 7); SetColor(14); cout << "[Batch Edit - Bo trong an Enter de giu nguyen]"; SetColor(15);
                
                gotoxy(10, 9); cout << "Ho (" << sNode->data.HO << "): ";
                std::string newHo = ReadInput(30, true, false);
                if (!newHo.empty()) sNode->data.HO = newHo;
                
                gotoxy(10, 10); cout << "Ten (" << sNode->data.TEN << "): ";
                std::string newTen = ReadInput(15, false, false);
                if (!newTen.empty()) sNode->data.TEN = newTen;
                
                while(true) {
                    gotoxy(10, 11); cout << "Phai (" << sNode->data.PHAI << "): ";
                    std::string newPhai = ReadInput(5, false, false);
                    if (newPhai.empty()) break; // Keep old
                    if (newPhai == "Nam" || newPhai == "nam") { sNode->data.PHAI = "Nam"; break; }
                    if (newPhai == "Nu" || newPhai == "nu") { sNode->data.PHAI = "Nu"; break; }
                    gotoxy(10, 12); SetColor(12); cout << "Loi: Chi nhap Nam hoac Nu!"; SetColor(15);
                    Sleep(1000); gotoxy(10, 12); cout << "                               ";
                    gotoxy(25, 11); cout << "       "; 
                }
                
                gotoxy(10, 13); cout << "Password (***): ";
                std::string newPass = ReadInput(15, false, true);
                if (!newPass.empty()) sNode->data.password = newPass;
                
                gotoxy(10, 15); SetColor(10); cout << "Cap nhat thanh cong!";
            } else {
                gotoxy(10, 7); SetColor(12); cout << "Loi: Sinh vien khong thuoc lop nay!";
            }
            SetColor(15); Sleep(1500);
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 7. MANAGE CLASSES & STUDENTS ROOT MENU
// ==========================================
void ManageClassesAndStudentsUI(ClassList& listClasses) {
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11); gotoxy(25, 2); cout << "=== QUAN LY LOP VA SINH VIEN ==="; SetColor(15);
        
        gotoxy(10, 5); cout << "1. Quan ly Lop hoc";
        gotoxy(10, 6); cout << "2. Quan ly Sinh vien";
        gotoxy(10, 8); cout << "0. Quay lai";
        gotoxy(10, 10); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            ManageClassesUI(listClasses);
        } else if (choice == "2") {
            ManageStudentsUI(listClasses);
        } else if (choice == "0") {
            isRunning = false;
        }
    }
}

// ==========================================
// 8. REPORT FUNCTIONS (Requirements h & i)
// ==========================================

// Requirement h: Print detailed exam questions of a student for 1 subject
void PrintDetailedExamResults(ClassList& listClasses, SubjectTree rootSubjects) {
    system("cls");
    SetColor(11); gotoxy(10, 3); cout << "=== IN CHI TIET BAI THI CUA SINH VIEN ==="; SetColor(15);
    
    gotoxy(10, 5); cout << "Nhap Ma Lop: ";
    string malop = ToUpper(ReadInput(15, false, false));
    if (malop.empty()) return;
    Class* c = FindClass(listClasses, malop);
    if (c == nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Lop khong ton tai!"; SetColor(15);
        Sleep(1500); return;
    }
    
    gotoxy(10, 7); cout << "Nhap Ma Sinh Vien: ";
    string masv = ToUpper(ReadInput(15, false, false));
    if (masv.empty()) return;
    StudentNode* st = FindStudent(c->students, masv);
    if (st == nullptr) {
        gotoxy(10, 9); SetColor(12); cout << "Loi: Sinh vien khong thuoc lop nay!"; SetColor(15);
        Sleep(1500); return;
    }

    gotoxy(10, 9); cout << "Nhap Ma Mon Hoc: ";
    string mamh = ToUpper(ReadInput(15, false, false));
    if (mamh.empty()) return;
    SubjectNode* sub = SearchSubject(rootSubjects, mamh);
    if (sub == nullptr) {
        gotoxy(10, 11); SetColor(12); cout << "Loi: Mon hoc khong ton tai!"; SetColor(15);
        Sleep(1500); return;
    }

    ScoreNode* sc = FindScore(st->data.scores, sub->data.MAMH);
    if (sc == nullptr) {
        system("cls");
        gotoxy(10, 5); SetColor(12);
        cout << "Sinh vien (" << st->data.MASV << " - " << st->data.HO << " " << st->data.TEN << ") CHUA THI mon (" << sub->data.TENMH << ")!";
        SetColor(15); Sleep(2000);
        return;
    }

    system("cls");
    SetColor(11);
    gotoxy(10, 2); cout << "=== CHI TIET BAI THI MON: " << sub->data.TENMH << " ===";
    SetColor(14);
    gotoxy(10, 3); cout << "Sinh vien: " << st->data.MASV << " - " << st->data.HO << " " << st->data.TEN;
    gotoxy(10, 4); cout << "Diem thi: " << sc->data.Diem << " / 10.0";
    SetColor(15);

    int y = 6;
    int qIndex = 1;
    AnswerDetailNode* ad = sc->data.details;
    while (ad != nullptr) {
        string qContent = ad->data.content;
        string qA = ad->data.A;
        string qB = ad->data.B;
        string qC = ad->data.C;
        string qD = ad->data.D;
        char qAns = ad->data.answer;

        // Tương thích ngược: Nếu snapshot rỗng (dữ liệu cũ), tra cứu lại từ môn học
        if (qContent.empty()) {
            QuestionNode* q = FindQuestion(sub->data.questions, ad->data.questionId);
            if (q != nullptr) {
                qContent = q->data.content;
                qA = q->data.A;
                qB = q->data.B;
                qC = q->data.C;
                qD = q->data.D;
                qAns = q->data.answer;
            } else {
                qContent = "(Cau hoi ID " + to_string(ad->data.questionId) + " da bi xoa khoi ngan hang de)";
                qAns = '?';
            }
        }

        gotoxy(5, y); SetColor(14);
        cout << "Cau " << qIndex++ << ": " << qContent;
        SetColor(15); y++;

        gotoxy(8, y); cout << "A. " << qA; y++;
        gotoxy(8, y); cout << "B. " << qB; y++;
        gotoxy(8, y); cout << "C. " << qC; y++;
        gotoxy(8, y); cout << "D. " << qD; y++;

        gotoxy(8, y); cout << "SV chon: ";
        if (ad->data.studentSelection == qAns) {
            SetColor(10); cout << ad->data.studentSelection << " (DUNG)";
        } else {
            SetColor(12); cout << ad->data.studentSelection << " (SAI, Dap an dung: " << qAns << ")";
        }
        SetColor(15); y += 2;

        if (y > 18) {
            gotoxy(5, y + 1); cout << "Nhan phim bat ky de xem tiep...";
            getch();
            system("cls");
            SetColor(11); gotoxy(10, 2); cout << "=== CHI TIET BAI THI MON: " << sub->data.TENMH << " ===";
            SetColor(14); gotoxy(10, 3); cout << "Sinh vien: " << st->data.MASV << " - " << st->data.HO << " " << st->data.TEN;
            gotoxy(10, 4); cout << "Diem thi: " << sc->data.Diem << " / 10.0"; SetColor(15);
            y = 6;
        }
        ad = ad->next;
    }

    gotoxy(5, y + 1); cout << "Nhan phim bat ky de quay lai...";
    getch();
}

// Requirement i: Print class exam scores for 1 subject ("Chua thi" if student hasn't taken exam)
void PrintClassExamScores(ClassList& listClasses, SubjectTree rootSubjects) {
    system("cls");
    SetColor(11); gotoxy(10, 3); cout << "=== IN BANG DIEM THI THEO LOP ==="; SetColor(15);
    
    gotoxy(10, 5); cout << "Nhap Ma Lop: ";
    string malop = ToUpper(ReadInput(15, false, false));
    if (malop.empty()) return;
    Class* c = FindClass(listClasses, malop);
    if (c == nullptr) {
        gotoxy(10, 7); SetColor(12); cout << "Loi: Lop khong ton tai!"; SetColor(15);
        Sleep(1500); return;
    }

    gotoxy(10, 7); cout << "Nhap Ma Mon Hoc: ";
    string mamh = ToUpper(ReadInput(15, false, false));
    if (mamh.empty()) return;
    SubjectNode* sub = SearchSubject(rootSubjects, mamh);
    if (sub == nullptr) {
        gotoxy(10, 9); SetColor(12); cout << "Loi: Mon hoc khong ton tai!"; SetColor(15);
        Sleep(1500); return;
    }

    system("cls");
    SetColor(11);
    gotoxy(15, 2); cout << "=== BANG DIEM THI TRAC NGHIEM ===";
    SetColor(14);
    gotoxy(15, 3); cout << "LOP: " << c->MALOP << " - " << c->TENLOP;
    gotoxy(15, 4); cout << "MON HOC: " << sub->data.MAMH << " - " << sub->data.TENMH;
    SetColor(15);

    gotoxy(5, 6); cout << "STT";
    gotoxy(12, 6); cout << "MASV";
    gotoxy(25, 6); cout << "HO";
    gotoxy(45, 6); cout << "TEN";
    gotoxy(60, 6); cout << "DIEM THI";
    gotoxy(5, 7); cout << "-------------------------------------------------------------------------";

    int y = 8;
    int index = 1;
    StudentNode* st = c->students;
    if (st == nullptr) {
        gotoxy(5, y); cout << "(Lop chua co sinh vien nao)";
    }

    while (st != nullptr) {
        gotoxy(5, y); cout << index++;
        gotoxy(12, y); cout << st->data.MASV;
        gotoxy(25, y); cout << st->data.HO;
        gotoxy(45, y); cout << st->data.TEN;

        ScoreNode* sc = FindScore(st->data.scores, sub->data.MAMH);
        gotoxy(60, y);
        if (sc != nullptr) {
            SetColor(10); cout << sc->data.Diem; SetColor(15);
        } else {
            SetColor(12); cout << "Chua thi"; SetColor(15);
        }
        y++;
        st = st->next;

        if (y > 22) {
            gotoxy(5, y + 1); cout << "Nhan phim bat ky de xem tiep...";
            getch();
            system("cls");
            SetColor(11); gotoxy(15, 2); cout << "=== BANG DIEM THI TRAC NGHIEM ===";
            SetColor(14); gotoxy(15, 3); cout << "LOP: " << c->MALOP << " - " << c->TENLOP;
            gotoxy(15, 4); cout << "MON HOC: " << sub->data.MAMH << " - " << sub->data.TENMH;
            SetColor(15);
            gotoxy(5, 6); cout << "STT"; gotoxy(12, 6); cout << "MASV"; gotoxy(25, 6); cout << "HO"; gotoxy(45, 6); cout << "TEN"; gotoxy(60, 6); cout << "DIEM THI";
            gotoxy(5, 7); cout << "-------------------------------------------------------------------------";
            y = 8;
        }
    }

    gotoxy(5, y + 2); cout << "Nhan phim bat ky de quay lai...";
    getch();
}

// ==========================================
// 9. MAIN ENTRY: TEACHER MENU
// ==========================================
void TeacherMainMenu(SubjectTree& rootSubjects, ClassList& listClasses) {
    bool isRunning = true;
    while (isRunning) {
        system("cls");
        SetColor(11);
        gotoxy(30, 4); cout << "=== MENU GIAO VIEN ===";
        
        SetColor(15);
        gotoxy(30, 7); cout << "1. Quan ly Mon hoc (BST)";
        gotoxy(30, 9); cout << "2. Quan ly Lop & Sinh Vien";
        gotoxy(30, 11); cout << "3. Quan ly Cau hoi thi (Linked List)";
        gotoxy(30, 13); cout << "4. In chi tiet cau hoi da thi cua 1 Sinh vien (Muc h)";
        gotoxy(30, 15); cout << "5. In bang diem thi môn hoc cua 1 Lop (Muc i)";
        gotoxy(30, 17); cout << "0. Quay lai Menu Chinh";
        gotoxy(30, 20); cout << "Chon: ";
        
        std::string choice = ReadInput(1, false, false);
        
        if (choice == "1") {
            ManageSubjectsUI(rootSubjects);
        }
        else if (choice == "2") {
            ManageClassesAndStudentsUI(listClasses);
        }
        else if (choice == "3") {
            ManageQuestionsUI(rootSubjects);
        }
        else if (choice == "4") {
            PrintDetailedExamResults(listClasses, rootSubjects);
        }
        else if (choice == "5") {
            PrintClassExamScores(listClasses, rootSubjects);
        }
        else if (choice == "0") {
            isRunning = false;
        }
    }
}
