#pragma once
#include <string>

using namespace std;

// ==========================================
// 1. DANH SACH CAU HOI THI (Singly Linked List)
// ==========================================
// Sinh vien beginner note: 
// - We use a Singly Linked List for questions because a subject can have any number of questions.
// - Pointers (*next) link one question to the next in memory dynamically.
struct Question {
    int id;                 // Unique auto-incrementing ID across all subjects
    string content;         // Question statement
    string A, B, C, D;      // Four options
    char answer;            // Correct key: 'A', 'B', 'C', or 'D'
};

struct QuestionNode {
    Question data;
    QuestionNode* next = nullptr; // Pointer to the next question
};

// ==========================================
// 2. DANH SACH MON HOC (Binary Search Tree - BST)
// ==========================================
// Sinh vien beginner note:
// - BST allows extremely fast O(log N) searching by Subject Code (MAMH).
// - Each node has a left and right child to maintain alphabetical order.
struct Subject {
    string MAMH;            // Subject Code (Unique)
    string TENMH;           // Subject Name
    QuestionNode* questions = nullptr; // Pointer to Question List (Singly Linked List)
};

struct SubjectNode {
    Subject data;
    SubjectNode* left = nullptr;
    SubjectNode* right = nullptr;
};
// We define a cleaner type name for the root pointer
typedef SubjectNode* SubjectTree;

// ==========================================
// 3. DANH SACH CHI TIET DAP AN (Singly Linked List)
// ==========================================
// Tracks exactly what the student selected for each question during the exam
struct AnswerDetail {
    int questionId;          // The ID of the question asked
    char studentSelection;   // What the student answered ('A', 'B', 'C', 'D' or ' ')
};

struct AnswerDetailNode {
    AnswerDetail data;
    AnswerDetailNode* next = nullptr;
};

// ==========================================
// 4. DANH SACH DIEM THI (Singly Linked List)
// ==========================================
// Each student can take many exams, so we store their scores in a linked list.
struct Score {
    string Mamh;             // Subject Code they took
    float Diem;              // Their final score (0.0 -> 10.0)
    AnswerDetailNode* details = nullptr; // Pointer to detailed question-by-question answers
};

struct ScoreNode {
    Score data;
    ScoreNode* next = nullptr;
};

// ==========================================
// 5. DANH SACH SINH VIEN (Singly Linked List)
// ==========================================
struct Student {
    string MASV;            // Student ID (Unique)
    string HO;              // Last Name
    string TEN;             // First Name
    string PHAI;            // Gender ("Nam" or "Nu")
    string password;        // Login password
    ScoreNode* scores = nullptr; // Pointer to their score list
};

struct StudentNode {
    Student data;
    StudentNode* next = nullptr;
};

// ==========================================
// 6. DANH SACH LOP (Array of Pointers)
// ==========================================
// Sinh vien beginner note:
// - The requirement states an Array of Pointers (max 10,000). 
// - We create a static array of pointers, and only allocate memory (new) when adding a class.
const int MAX_CLASSES = 10000;
struct Class {
    string MALOP;           // Class Code (Unique)
    string TENLOP;          // Class Name
    StudentNode* students = nullptr; // Pointer to Student List for this class
};

struct ClassList {
    Class* nodes[MAX_CLASSES]; // Array of pointers to Class structs
    int size = 0;              // Current number of classes
};
