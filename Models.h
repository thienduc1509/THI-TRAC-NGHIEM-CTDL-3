#pragma once
#include <string>

using namespace std;

// ==========================================
// 1. DANH SACH CAU HOI THI (Singly Linked List)
// ==========================================
struct Question {
    int id;                 // Unique auto-incrementing ID across all subjects
    string content;         // Question statement
    string A, B, C, D;      // Four options
    char answer;            // Correct key: 'A', 'B', 'C', or 'D'
};

struct QuestionNode {
    Question data;
    QuestionNode* next = nullptr; // Pointer to the next question

    QuestionNode() : next(nullptr) {}
    QuestionNode(const Question& d, QuestionNode* n = nullptr) : data(d), next(n) {}
};

// ==========================================
// 2. DANH SACH MON HOC (Binary Search Tree - BST)
// ==========================================
struct Subject {
    string MAMH;            // Subject Code (Unique)
    string TENMH;           // Subject Name
    QuestionNode* questions = nullptr; // Pointer to Question List (Singly Linked List)
};

struct SubjectNode {
    Subject data;
    SubjectNode* left = nullptr;
    SubjectNode* right = nullptr;

    SubjectNode() : left(nullptr), right(nullptr) {}
    SubjectNode(const Subject& d) : data(d), left(nullptr), right(nullptr) {}
};
typedef SubjectNode* SubjectTree;

// ==========================================
// 3. DANH SACH CHI TIET DAP AN (Singly Linked List)
// ==========================================
// Snapshot của từng câu hỏi đã thi (giúp in lại bài thi mục h chính xác ngay cả khi GV xóa/sửa câu hỏi)
struct AnswerDetail {
    int questionId;          // ID câu hỏi
    string content;          // Nội dung câu hỏi tại thời điểm thi
    string A, B, C, D;      // Các đáp án A, B, C, D tại thời điểm thi
    char answer;            // Đáp án đúng
    char studentSelection;   // SV chọn ('A', 'B', 'C', 'D' hoặc ' ')
};

struct AnswerDetailNode {
    AnswerDetail data;
    AnswerDetailNode* next = nullptr;

    AnswerDetailNode() : next(nullptr) {}
    AnswerDetailNode(const AnswerDetail& d, AnswerDetailNode* n = nullptr) : data(d), next(n) {}
};

// ==========================================
// 4. DANH SACH DIEM THI (Singly Linked List)
// ==========================================
struct Score {
    string Mamh;             // Mã môn học đã thi
    float Diem;              // Điểm số (0.0 -> 10.0)
    AnswerDetailNode* details = nullptr; // Con trỏ đến danh sách chi tiết các câu đã thi
};

struct ScoreNode {
    Score data;
    ScoreNode* next = nullptr;

    ScoreNode() : next(nullptr) {}
    ScoreNode(const Score& d, ScoreNode* n = nullptr) : data(d), next(n) {}
};

// ==========================================
// 5. DANH SACH SINH VIEN (Singly Linked List)
// ==========================================
struct Student {
    string MASV;            // Mã sinh viên (Duy nhất)
    string HO;              // Họ và tên lót
    string TEN;             // Tên sinh viên
    string PHAI;            // Giới tính ("Nam" hoặc "Nu")
    string password;        // Mật khẩu đăng nhập
    ScoreNode* scores = nullptr; // Con trỏ đến danh sách điểm các môn đã thi
};

struct StudentNode {
    Student data;
    StudentNode* next = nullptr;

    StudentNode() : next(nullptr) {}
    StudentNode(const Student& d, StudentNode* n = nullptr) : data(d), next(n) {}
};

// ==========================================
// 6. DANH SACH LOP (Array of Pointers)
// ==========================================
const int MAX_CLASSES = 10000;
struct Class {
    string MALOP;           // Mã lớp (Duy nhất)
    string TENLOP;          // Tên lớp
    StudentNode* students = nullptr; // Con trỏ đến danh sách sinh viên thuộc lớp
};

struct ClassList {
    Class* nodes[MAX_CLASSES]; // Mảng chứa tối đa 10.000 con trỏ Lớp
    int size = 0;              // Số lượng lớp hiện tại
};
