#pragma once
#include "Models.h"

// ==========================================
// ANSWER DETAIL MANAGEMENT
// ==========================================
void AddAnswerDetail(AnswerDetailNode*& head, AnswerDetail data);
void FreeAnswerDetailList(AnswerDetailNode*& head);

// ==========================================
// SCORE MANAGEMENT
// ==========================================
void AddScore(ScoreNode*& head, Score data);
ScoreNode* FindScore(ScoreNode* head, string mamh);
void FreeScoreList(ScoreNode*& head);

// ==========================================
// STUDENT MANAGEMENT
// ==========================================
void AddStudent(StudentNode*& head, Student data);
StudentNode* FindStudent(StudentNode* head, string masv);
// Helper to find a student across ALL classes (Returns pointer and sets belongingClass)
StudentNode* FindStudentGlobal(ClassList& dsLop, string masv, Class*& belongingClass);
void FreeStudentList(StudentNode*& head);

// ==========================================
// CLASS MANAGEMENT
// ==========================================
void InitClassList(ClassList& dsLop);
bool AddClass(ClassList& dsLop, Class data);
Class* FindClass(ClassList& dsLop, string malop);
Class* FindClassByName(ClassList& dsLop, string tenlop);
void FreeClassList(ClassList& dsLop);
