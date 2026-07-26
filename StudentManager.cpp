#include "StudentManager.h"

// ==========================================
// ANSWER DETAIL MANAGEMENT
// ==========================================

void AddAnswerDetail(AnswerDetailNode*& head, AnswerDetail data) {
    AnswerDetailNode* newNode = new AnswerDetailNode(data, nullptr);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    AnswerDetailNode* p = head;
    while (p->next != nullptr) p = p->next;
    p->next = newNode;
}
void FreeAnswerDetailList(AnswerDetailNode*& head) {
    while (head != nullptr) {
        AnswerDetailNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// ==========================================
// SCORE MANAGEMENT
// ==========================================

void AddScore(ScoreNode*& head, Score data) {
    ScoreNode* newNode = new ScoreNode(data, nullptr);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    ScoreNode* p = head;
    while (p->next != nullptr) p = p->next;
    p->next = newNode;
}

ScoreNode* FindScore(ScoreNode* head, string mamh) {
    while (head != nullptr) {
        if (head->data.Mamh == mamh) return head;
        head = head->next;
    }
    return nullptr;
}

void FreeScoreList(ScoreNode*& head) {
    while (head != nullptr) {
        ScoreNode* temp = head;
        head = head->next;
        FreeAnswerDetailList(temp->data.details);
        delete temp;
    }
}

// ==========================================
// STUDENT MANAGEMENT
// ==========================================

// Sorted Insert for Student Singly Linked List (Ascending by TEN -> HO -> MASV)
void AddStudent(StudentNode*& head, Student data) {
    StudentNode* newNode = new StudentNode(data, nullptr);
    string keyNew = data.TEN + " " + data.HO + " " + data.MASV;
    
    if (head == nullptr || (head->data.TEN + " " + head->data.HO + " " + head->data.MASV) >= keyNew) {
        newNode->next = head;
        head = newNode;
        return;
    }
    
    StudentNode* p = head;
    while (p->next != nullptr && (p->next->data.TEN + " " + p->next->data.HO + " " + p->next->data.MASV) < keyNew) {
        p = p->next;
    }
    newNode->next = p->next;
    p->next = newNode;
}

StudentNode* FindStudent(StudentNode* head, string masv) {
    while (head != nullptr) {
        if (head->data.MASV == masv) return head;
        head = head->next;
    }
    return nullptr;
}

StudentNode* FindStudentGlobal(ClassList& dsLop, string masv, Class*& belongingClass) {
    for (int i = 0; i < dsLop.size; i++) {
        StudentNode* found = FindStudent(dsLop.nodes[i]->students, masv);
        if (found != nullptr) {
            belongingClass = dsLop.nodes[i];
            return found;
        }
    }
    belongingClass = nullptr;
    return nullptr;
}

void FreeStudentList(StudentNode*& head) {
    while (head != nullptr) {
        StudentNode* temp = head;
        head = head->next;
        // MUST clear inner list (scores) first to prevent memory leak
        FreeScoreList(temp->data.scores); 
        delete temp;
    }
}

// ==========================================
// CLASS MANAGEMENT
// ==========================================

void InitClassList(ClassList& dsLop) {
    dsLop.size = 0;
    for (int i = 0; i < MAX_CLASSES; i++) {
        dsLop.nodes[i] = nullptr;
    }
}

// Sorted Insert for Class Pointer Array (Ascending by MALOP)
bool AddClass(ClassList& dsLop, Class data) {
    if (dsLop.size >= MAX_CLASSES) return false;
    
    // Check duplicate Class Code or Class Name
    if (FindClass(dsLop, data.MALOP) != nullptr) return false;
    if (FindClassByName(dsLop, data.TENLOP) != nullptr) return false;
    
    // Allocate new Class memory
    Class* newClass = new Class;
    newClass->MALOP = data.MALOP;
    newClass->TENLOP = data.TENLOP;
    newClass->students = nullptr; // Initialize list to empty
    
    // Shift elements right to keep array sorted by MALOP
    int i = dsLop.size - 1;
    while (i >= 0 && dsLop.nodes[i]->MALOP > data.MALOP) {
        dsLop.nodes[i + 1] = dsLop.nodes[i];
        i--;
    }
    dsLop.nodes[i + 1] = newClass;
    dsLop.size++;
    return true;
}

Class* FindClass(ClassList& dsLop, string malop) {
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr && dsLop.nodes[i]->MALOP == malop) {
            return dsLop.nodes[i];
        }
    }
    return nullptr;
}

Class* FindClassByName(ClassList& dsLop, string tenlop) {
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr && dsLop.nodes[i]->TENLOP == tenlop) {
            return dsLop.nodes[i];
        }
    }
    return nullptr;
}

void FreeClassList(ClassList& dsLop) {
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr) {
            // MUST clear inner list (students) first to prevent memory leak
            FreeStudentList(dsLop.nodes[i]->students); 
            delete dsLop.nodes[i];
            dsLop.nodes[i] = nullptr;
        }
    }
    dsLop.size = 0;
}
